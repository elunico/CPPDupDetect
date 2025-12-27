#ifndef DUPDETECT_FILEUTILS_HPP
#define DUPDETECT_FILEUTILS_HPP

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "fllock.hpp"
#include "shautils.hpp"

template <typename T>
std::size_t count_all(std::filesystem::path const& path, T const& pred)
{
    std::size_t count = 0;
    for (auto const& entry :
         std::filesystem::recursive_directory_iterator(path)) {
        if (pred(entry)) {
            ++count;
        }
    }
    return count;
}

struct DirectoryHasher {
    using PathType = std::string;
    using HashType = std::string;

    PathType                                            path;
    std::unordered_map<HashType, std::vector<PathType>> duplicates;

    std::size_t get_progress() const
    {
        return progress;
    }

    std::size_t get_total() const
    {
        return total;
    }

    DirectoryHasher(const std::string& p)
        : path(p),
          iterator(std::filesystem::recursive_directory_iterator(p)),
          progress(0),
          total(count_all(p, [](const std::filesystem::directory_entry& entry) {
              return entry.is_regular_file();
          }))
    {
    }

    std::optional<PathType> will_be_hashed()
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

    std::optional<std::tuple<PathType, HashType>> next()
    {
        if (iterator == std::filesystem::end(iterator)) {
            return std::nullopt;
        }

        auto& entry = *iterator;
        char  outputBuffer[65];
        if (entry.is_regular_file()) {
            sha256_file(const_cast<char*>(entry.path().string().c_str()),
                        outputBuffer);
            duplicates[std::string(outputBuffer)].push_back(
                entry.path().string());
            ++progress;
            auto tup = std::make_tuple(entry.path().string(),
                                       std::string(outputBuffer));
            ++iterator;
            return tup;
        } else {
            auto tup = std::make_tuple(entry.path().string(), "<directory>");
            ++iterator;
            return tup;
        }
    }

   private:
    std::filesystem::recursive_directory_iterator iterator;
    std::size_t                                   progress;
    std::size_t                                   total;
};

#endif  // DUPDETECT_FILEUTILS_HPP
