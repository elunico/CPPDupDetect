#ifndef DUPDETECT_FILEUTILS_HPP
#define DUPDETECT_FILEUTILS_HPP

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

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

    std::size_t get_progress() const;

    std::size_t get_total() const;

    DirectoryHasher(const std::string& p);

    std::optional<PathType> will_be_hashed();

    std::optional<std::tuple<PathType, HashType>> next();

   private:
    std::filesystem::recursive_directory_iterator iterator;
    std::size_t                                   progress;
    std::size_t                                   total;
};

#endif  // DUPDETECT_FILEUTILS_HPP
