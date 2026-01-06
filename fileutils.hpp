#ifndef DUPDETECT_FILEUTILS_HPP
#define DUPDETECT_FILEUTILS_HPP

#include <atomic>
#include <exception>
#include <filesystem>
#include <limits>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utils.hpp>
#include <vector>

std::filesystem::file_time_type last_write_time_safe(std::string const& entry);

struct file_hash_error: std::exception {
    private:
        char const *reason;
    public:
        char const* what() const noexcept override; 
    explicit file_hash_error(char const* reason);
    ~file_hash_error();  
};

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

template <typename PathType, typename HashType>
struct HashEntry {
    HashType              hash;
    std::size_t           byte_count;
    std::vector<PathType> files;

    bool matches(HashType const& hash) const
    {
        return this->hash == hash;
    }

    void add_file(PathType const& path)
    {
        auto file_size = std::filesystem::file_size(path);
        // Check for overflow before adding
        if (byte_count > std::numeric_limits<std::size_t>::max() - file_size) {
            ::output("Warning: byte_count overflow prevented for ", path);
            // Don't add to byte_count if it would overflow
        } else {
            byte_count += file_size;
        }
        files.push_back(path);
    }

    std::size_t get_file_count() const
    {
        return files.size();
    }
};

struct DirectoryHasher {
    using PathType = std::string;
    using HashType = std::string;

    using DuplicateFilesCollection = std::unordered_map<HashType, HashEntry<PathType, HashType>>;
    PathType                                                    path;
    std::shared_ptr<DuplicateFilesCollection> duplicates;

    [[nodiscard]] std::size_t get_progress() const;

    [[nodiscard]] std::size_t get_total() const;

    explicit DirectoryHasher(const std::string& p);

    std::optional<PathType> will_be_hashed();

    std::optional<std::tuple<PathType, HashType>> next();

   private:
    std::filesystem::recursive_directory_iterator iterator;
    std::atomic<std::size_t>                      progress;
    std::size_t                                   total;
    std::mutex                                    iterator_mutex;
    std::mutex                                    duplicates_mutex;
};

#endif  // DUPDETECT_FILEUTILS_HPP
