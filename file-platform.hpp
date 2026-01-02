
#ifdef WIN32

extern "C" {

#include <shellapi.h>

inline static void reveal_file(char const* path)
{
    auto result = ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);

    // see
    // https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shellexecutea?redirectedfrom=MSDN
    if (result <= 32) {
        throw std::runtime_error("Cannot open path");
    }
}
}

#elif defined(__APPLE__)

#include <string>
#include <format>

extern "C" {

#include <unistd.h>

inline static void reveal_file(char const* path)
{
    std::string command = std::format("open -R \"{}\"", path);
    system(command.c_str());    
}

}

#elif defined(__LINUX__) 

inline static void reveal_file(char const* path)
{
    std::string command = std::format("nautilus \"{}\"", path);
    system(command.c_str());
}

#else

#include <iostream>
#warning "Platform unknown. reveal_file will not function"

inline static void reveal_file(char const* path)
{
    std::cerr << "Cannot call reveal_file() on this platform" << std::endl;
}   


#endif
