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
   auto result = fl_choice("%s", fl_no, fl_yes, 0, buf);
   if (result == 1) {
     return ConfirmResult::YES; 
   }
   return ConfirmResult::NO;
}

#endif 
