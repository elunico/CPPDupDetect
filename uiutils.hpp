#ifndef UIUTILS_HPP
#define UIUTILS_HPP

#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <cstdio>

enum struct ConfirmResult {
  YES, NO 
};

template<typename... Args>
inline static ConfirmResult confirm(char const* fmt, Args&&... args)
{
   char buf[1 << 12];
   std::snprintf(buf, 1 << 12, fmt, (args, ...));
   auto result = fl_ask(buf);
   if (result == 1) {
     return ConfirmResult::YES; 
   }
   return ConfirmResult::NO;
}

#endif 
