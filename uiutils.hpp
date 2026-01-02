#ifndef UIUTILS_HPP
#define UIUTILS_HPP

#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <cstdio>

struct ConfirmToken {
    bool auto_yes = false;
};

enum struct ConfirmResult { NO, YES };

template <typename... Args>
inline static ConfirmResult confirm(ConfirmToken* token,
                                    char const*   fmt,
                                    Args&&... args)
{
    if (token && token->auto_yes) {
        return ConfirmResult::YES;
    }
    char const* third_button = token == nullptr ? 0 : "Yes and Don't Ask Again";
    auto        result = fl_choice(fmt, fl_no, fl_yes, third_button, args...);
    if (result == 1 || result == 2) {
        if (token && result == 2) {
            token->auto_yes = true;
        }
        return ConfirmResult::YES;
    }
    return ConfirmResult::NO;
}

template <typename... Args>
inline static ConfirmResult confirm(char const* fmt, Args&&... args)
{
    return confirm(static_cast<ConfirmToken*>(nullptr), fmt, args...);
}

#endif
