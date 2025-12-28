#include "fileutils.hpp"
#include "shautils.hpp"

std::size_t DirectoryHasher::get_progress() const
{
    return progress;
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
    if (iterator == std::filesystem::end(iterator)) {
        return std::nullopt;
    }

    auto& entry = *iterator;
    char  outputBuffer[65];
    if (entry.is_regular_file()) {
        sha256_file(const_cast<char*>(entry.path().string().c_str()),
                    outputBuffer);
        duplicates[std::string(outputBuffer)].push_back(entry.path().string());
        ++progress;
        auto tup =
            std::make_tuple(entry.path().string(), std::string(outputBuffer));
        ++iterator;
        return tup;
    } else {
        auto tup = std::make_tuple(entry.path().string(), "<directory>");
        ++iterator;
        return tup;
    }
}
