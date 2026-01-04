#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <exception>

[[noreturn]] static inline void die()
{
    std::terminate();
}

#ifndef SILENCE_OUTPUT

template <typename... Args>
void output(Args&&... args)
{
    ((std::cout << std::forward<Args>(args)), ...);
    std::cout << '\n';
}

#else

template <typename... Args>
inline constexpr void output([[maybe_unused]] Args&&... args)
{
    return;
}

#endif

#ifdef NDEBUG

#define debug_output(...) (void) 0

#define DEBUG_USE_ONLY [[maybe_unused]]

#else

#define DEBUG_USE_ONLY

#define debug_output(...) ::output(__VA_ARGS__)

#endif  // NDEBUG

#endif  // UTILS_HPP
