#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

#ifdef NDEBUG

template<typename... Args>
constexpr inline void output(Args&&... args)
{
  (void)0;
}

#else

template<typename... Args>
void output(Args&&... args)
{
  ((std::cout << std::forward<Args>(args)), ...); 
  std::cout << '\n';
}

#endif // NDEBUG

#endif // UTILS_HPP
