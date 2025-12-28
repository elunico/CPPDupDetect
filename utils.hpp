#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef NDEBUG

template <typename... Args>
constexpr inline void output([[maybe_unused]] Args&&... args)
{
    (void) 0;
}

#else

#include <iostream>

template <typename... Args>
void output(Args&&... args)
{
    ((std::cout << std::forward<Args>(args)), ...);
    std::cout << '\n';
}

#endif  // NDEBUG

#endif  // UTILS_HPP
