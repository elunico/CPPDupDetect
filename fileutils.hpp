#ifndef DUPDETECT_FILEUTILS_HPP
#define DUPDETECT_FILEUTILS_HPP

#include <filesystem>
#include <string>
#include <unordered_map>
#include <utils.hpp>
#include <vector>

template <typename T>
std::size_t count_all(std::filesystem::path const& path, T const& pred)
{
    std::size_t count    = 0;
    auto        iterator = std::filesystem::recursive_directory_iterator(path);
    while (iterator != std::filesystem::end(iterator)) {
        if (auto& entry = *iterator; pred(entry)) {
            ++count;
        }
        try {
            ++iterator;
        } catch (const std::filesystem::filesystem_error&) {
            // Ignore errors and continue
            ::output("Warning: Unable to access path. Skipping.");
        }
    }
    return count;
}

struct DirectoryHasher {
    using PathType = std::string;
    using HashType = std::string;

    PathType                                            path;
    std::unordered_map<HashType, std::vector<PathType>> duplicates;

    [[nodiscard]] std::size_t get_progress() const;

    [[nodiscard]] std::size_t get_total() const;

    explicit DirectoryHasher(const std::string& p);

    std::optional<PathType> will_be_hashed();

    std::optional<std::tuple<PathType, HashType>> next();

   private:
    std::filesystem::recursive_directory_iterator iterator;
    std::size_t                                   progress;
    std::size_t                                   total;
};

#endif  // DUPDETECT_FILEUTILS_HPP
