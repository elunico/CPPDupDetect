#ifndef FILEDATA_HPP
#define FILEDATA_HPP

#include <string>
struct FileData {
    enum class Type { ROOT, DIRECTORY, FILE } type;
    std::string path;
    std::string hash;
    bool        survivor;
};

#endif
