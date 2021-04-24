#ifndef LUNAR_DEBUG_H
#define LUNAR_DEBUG_H

#include <fmt/format.h>
#include <fmt/color.h>
#include <string>

namespace Lunar {
    template <typename S, typename... Args>
    inline void log(const S& format_str, Args&&... args) 
    {
        std::string formatted = fmt::format(format_str, args...);
        fmt::print("[INFO] {}\n", formatted);
    }

    template <typename S, typename... Args>
    inline void warn(const S& format_str, Args&&... args)
    {
        std::string formatted = fmt::format(format_str, args...);
        fmt::print(fg(fmt::color::light_yellow), "[WARNING] {}\n", formatted);
    }

    template <typename S, typename... Args>
    inline void error(const S& format_str, Args&&... args)
    {
        std::string formatted = fmt::format(format_str, args...);
        fmt::print(fg(fmt::color::red), "[ERROR] {}\n", formatted);
    }
}

#endif // LUNAR_DEBUG_H
