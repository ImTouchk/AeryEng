#ifndef LUNAR_DEBUG_H
#define LUNAR_DEBUG_H

#include <fmt/color.h>

#define LUNAR_DEBUG

fmt::color _PrintColor = fmt::color::white;
const char* _PrintTitle = "INFO";

namespace Lunar {
    void PrintTitle(const char* NewTitle)
    {
#   ifdef LUNAR_DEBUG
        _PrintTitle = NewTitle;
#   endif
    }

    void PrintColor(fmt::color NewColor)
    {
#   ifdef LUNAR_DEBUG
        _PrintColor = NewColor;
#   endif
    }

    template<typename S, typename... Args>
    void Print(const S& format_str, Args&&... args)
    {
#   ifdef LUNAR_DEBUG
        std::string Result = fmt::format(format_str, args...);
        fmt::print(
            fmt::fg(_PrintColor),
            "[{}] {}\n", _PrintTitle, Result
        );
#   endif
    }

    template<typename S, typename... Args>
    void Warn(const S& format_str, Args&&... args)
    {
#   ifdef LUNAR_DEBUG
        std::string Result = fmt::format(format_str, args...);
        fmt::print(
            fmt::fg(fmt::color::yellow),
            "[WARNING] {}\n", Result
        );
#   endif
    }

    template<typename S, typename... Args>
    void Error(const S& format_str, Args&&... args)
    {
#   ifdef LUNAR_DEBUG
        std::string Result = fmt::format(format_str, args...);
        fmt::print(
            fmt::fg(fmt::color::orange_red),
            "[ERROR] {}\n", Result
        );
#   endif
    }

    void Exit()
    {
        fmt::print(
            fmt::fg(fmt::color::red),
            "[CRITICAL] Program exit called.\n"
        );
        exit(-1);
    }

    void Exit();
}

#endif