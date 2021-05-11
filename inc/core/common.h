#ifndef _LUNAR_CORE_COMMON_H
#define _LUNAR_CORE_COMMON_H

#include <fmt/core.h>
#include <fmt/color.h>

using isize = long;
using fsize = double;
using usize = unsigned long;
using byte  = unsigned char;

namespace Lunar
{
    template<typename S, typename ...Args>
    void print(const S& string, Args&&... args)
    {
#   ifndef NDEBUG
        std::string formatted = fmt::format(string, args...);
        fmt::print(
            fmt::fg(fmt::color::white), 
            "[INFO] {}\n", 
            formatted
        );
#   endif
    }

    template<typename S, typename ...Args>
    void error(const S& string, Args&&... args)
    {
#   ifndef NDEBUG
        std::string formatted = fmt::format(string, args...);
        fmt::print(
            fmt::fg(fmt::color::red), 
            "[ERROR] {}\n", 
            formatted
        );
#   endif
    }
}

#endif // _LUNAR_CORE_COMMON_H
