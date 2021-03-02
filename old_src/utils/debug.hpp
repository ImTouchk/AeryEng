#pragma once

#include <fmt/color.h>
#include <fmt/core.h>
#include <string>

namespace Aery {
    void log(std::string, fmt::color = fmt::color::white);
    void warn(std::string);
    void error(std::string);
    
    template <typename S, typename... Args, typename Char = fmt::char_t<S>>
    inline std::basic_string<Char> debug_format(const S& format_str, Args&&... args) {
#ifndef NDEBUG
        return fmt::format(format_str, args);
#else
        return "";
#endif
    }
}