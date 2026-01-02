#ifndef FILE_PLATFORM_H
#define FILE_PLATFORM_H

using PlatformUnavailableCallback = void (*)(char const*);

extern PlatformUnavailableCallback on_platform_unavailable;
void set_on_platform_unavailable(PlatformUnavailableCallback callback);

#ifdef WIN32

extern "C" {

#include <Windows.h>
#include <shellapi.h>
#include <format>
#include <stdexcept>

inline static void reveal_file(char const* path)
{
   ShellExecuteA(NULL, "open", "explorer.exe", std::format("/select \"{}\"", path).c_str(), NULL, SW_SHOWDEFAULT);

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
    std::string command = std::format("open -R \"{}\"", path);
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
    std::string command = std::format("nautilus \"{}\"", path);
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
