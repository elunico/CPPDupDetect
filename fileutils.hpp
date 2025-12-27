#ifndef DUPDETECT_FILEUTILS_HPP
#define DUPDETECT_FILEUTILS_HPP

#include <filesystem>
#include <iostream>
#include <map>
#include <string>
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

    PathType                                  path;
    std::map<HashType, std::vector<PathType>> duplicates;

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
        if (!is_first) {
            // Only invalidate the iterator after completing the step. but let the first entry happen.
            ++iterator;
        } else {
            is_first = false;
        }
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
        return std::make_tuple(entry.path().string(),
                               std::string(outputBuffer));
        } else {
            return std::make_tuple(entry.path().string(), "<directory>");
        }
    }

   private:
       bool is_first = true;
    std::filesystem::recursive_directory_iterator iterator;
    std::size_t                                   progress;
    std::size_t                                   total;
};

template <typename T, typename U>
void scan_directory(const std::string&                               path,
                    std::map<std::string, std::vector<std::string>>& duplicates,
                    T                                                onProgress,
                    U                                                keepScan)
{
    std::cout << "Scanning directory: " << path << std::endl;

    {
        FLLock l;
        onProgress(0, false);
    }
    auto file_iterator = std::filesystem::recursive_directory_iterator(path);
    std::map<uintmax_t, std::vector<std::string>> size_map;
    std::size_t                                   progress = 0;
    for (const auto& entry : file_iterator) {
        if (!keepScan()) {
            std::cout << "Scan cancelled." << std::endl;
            return;
        }
        if (entry.is_regular_file()) {
            uintmax_t file_size = entry.file_size();
            size_map[file_size].push_back(entry.path().string());
            char outputBuffer[65];
            sha256_file(const_cast<char*>(entry.path().string().c_str()),
                        outputBuffer);
            duplicates[std::string(outputBuffer)].push_back(
                entry.path().string());
            {
                FLLock l;
                onProgress(progress += 1, false);
            }
        }
    }
    std::cout << "Finished scanning directory: " << path << std::endl;
    FLLock l;
    onProgress(progress, true);
}

#endif  // DUPDETECT_FILEUTILS_HPP
