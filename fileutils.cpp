#include "fileutils.hpp"
#include <atomic>
#include <filesystem>
#include <mutex>
#include "shautils.hpp"
#include "utils.hpp"

std::filesystem::file_time_type last_write_time_safe(std::string const& entry)
{
    try {
        return std::filesystem::last_write_time(entry);
    } catch (...) {
        return std::filesystem::file_time_type{};
    }
}

std::size_t DirectoryHasher::get_progress() const
{
    return progress.load(std::memory_order_relaxed);
}

std::size_t DirectoryHasher::get_total() const
{
    return total;
}

DirectoryHasher::DirectoryHasher(const std::string& p)
    : path(p),
      iterator(std::filesystem::recursive_directory_iterator(p)),
      progress(0),
      total(count_all(p, [](const std::filesystem::directory_entry& entry) {
          return entry.is_regular_file();
      }))
{
}

std::optional<DirectoryHasher::PathType> DirectoryHasher::will_be_hashed()
{
    std::lock_guard guard{iterator_mutex};

    if (iterator == std::filesystem::end(iterator)) {
        return std::nullopt;
    }

    auto& entry = *iterator;
    if (entry.is_regular_file()) {
        return entry.path().string();
    } else {
        return std::nullopt;
    }
}

std::optional<std::tuple<DirectoryHasher::PathType, DirectoryHasher::HashType>>
DirectoryHasher::next()
{
    std::filesystem::directory_entry entry;

    {
        std::lock_guard guard{iterator_mutex};
        if (iterator == std::filesystem::end(iterator)) {
            return std::nullopt;
        }
        // copy the current entry to prevent race condition in pt thread.
        entry = *iterator;
    }

    char outputBuffer[65];
    try {
        if (entry.is_regular_file()) {
            sha256_file(entry.path().string().c_str(), outputBuffer);
            duplicates[std::string(outputBuffer)].push_back(
                entry.path().string());
            progress.fetch_add(1, std::memory_order_relaxed);

            {
                std::lock_guard guard{iterator_mutex};
                ++iterator;
            }
            return std::make_tuple(entry.path().string(),
                                   std::string(outputBuffer));
        } else {
            {
                std::lock_guard guard{iterator_mutex};
                ++iterator;
            }
            return std::make_tuple(entry.path().string(), "<directory>");
        }
    } catch (std::filesystem::filesystem_error& e) {
        // Skip files that cannot be accessed
        ::output("Warning: Could not access file. Skipping.");
        {
            std::lock_guard guard{iterator_mutex};
            while (iterator != std::filesystem::end(iterator)) {
                try {
                    ++iterator;
                    break;
                } catch (std::filesystem::filesystem_error& e) {
                    ::output("Warning skipping file");
                }
            }
        }
        // TODO: am i skipping one here?
        return next();
    }
}
