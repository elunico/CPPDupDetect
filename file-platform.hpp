#ifndef FILE_PLATFORM_H
#define FILE_PLATFORM_H


// WARNING: please do not clang-format this file. The order of #includes
// is semantically meaningful on Windows and clang-format can sort #includes

#include <cstring>
#include <string>
#include <unordered_set>

using PlatformUnavailableCallback = void (*)(char const*);

extern PlatformUnavailableCallback on_platform_unavailable;
void set_on_platform_unavailable(PlatformUnavailableCallback callback);

// Helper function to escape shell metacharacters for use in double-quoted strings
inline static std::string shell_escape(char const* path)
{
    static std::unordered_set<char> illegals{{'"', '\\', '$', '`', '@', '#', '(', ')', '!', '?', '|', '&'}};
    std::string result;
    result.reserve(std::strlen(path) * 2); // Reserve space for potential escaping

    for (const char* p = path; *p != '\0'; ++p) {
        // Escape characters that are special within double quotes
        if (illegals.contains(*p)) {
//        if (*p == '"' || *p == '\\' || *p == '$' || *p == '`') {
            result += '\\';
        }
        result += *p;
    }
    return result;
}

#ifdef WIN32

extern "C" {

#include <Windows.h>
#include <shellapi.h>
#include <format>
#include <stdexcept>

inline static void reveal_file(char const* path)
{
    // ShellExecuteA doesn't execute through cmd.exe, so it's safer
    // But we still should be cautious with the path
    std::string escaped = shell_escape(path);
    ShellExecuteA(NULL, "open", "explorer.exe", std::format("/select \"{}\"", escaped).c_str(), NULL, SW_SHOWDEFAULT);

    // see
    // https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shellexecutea?redirectedfrom=MSDN
    //if (result <= 32) {
    //    throw std::runtime_error("Cannot open path");
    //}
}
}

#elif defined(__APPLE__)

#include <format>
#include <string>

extern "C" {

#include <unistd.h>

inline static void reveal_file(char const* path)
{
    std::string escaped = shell_escape(path);
    std::string command = std::format("open -R \"{}\"", escaped);
    system(command.c_str());
}
}

#elif defined(__linux__)

#include <format>
#include <string>

extern "C" {

#include <unistd.h>

inline static void reveal_file(char const* path)
{
    std::string escaped = shell_escape(path);
    std::string command = std::format("nautilus \"{}\"", escaped);
    system(command.c_str());
}
}

#else

#include <iostream>
#warning "Platform unknown. reveal_file will not function"

inline static void reveal_file(char const* path)
{
    if (!on_platform_unavailable) {
        std::cerr << "Cannot call reveal_file() on this platform" << std::endl;
    } else {
        on_platform_unavailable(path);
    }
}

#endif

#endif
