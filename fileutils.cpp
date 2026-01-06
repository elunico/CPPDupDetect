#include "fileutils.hpp"
#include <atomic>
#include <exception>
#include <filesystem>
#include <mutex>
#include <stdexcept>
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

file_hash_error::file_hash_error(char const *reason): std::exception(), reason(reason) {
    
}

char const* file_hash_error::what() const noexcept {
    return reason; 
}

file_hash_error::~file_hash_error() = default; 

std::size_t DirectoryHasher::get_progress() const
{
    return progress.load(std::memory_order_acquire);
}

std::size_t DirectoryHasher::get_total() const
{
    return total;
}

DirectoryHasher::DirectoryHasher(const std::string& p)
    : path(p),
      duplicates(std::make_shared<DuplicateFilesCollection>()),
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
    if (entry.is_regular_file()) {
        int hash_result =
            sha256_file(entry.path().string().c_str(), outputBuffer);
        if (hash_result != 0) {
            // Failed to hash file, skip it
            {
                std::lock_guard guard{iterator_mutex};
                ++iterator; 
            }
            throw file_hash_error("Failed to perform file hashing");
        }
        {
            std::lock_guard guard{duplicates_mutex};
            auto& hashed = duplicates->operator[](std::string(outputBuffer));
            hashed.add_file(entry.path().string());
        }
        progress.fetch_add(1, std::memory_order_release);

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
}
