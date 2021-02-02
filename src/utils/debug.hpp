#pragma once

#include <fmt/color.h>
#include <string>

namespace Aery {
    void log(std::string, fmt::color = fmt::color::white);
    void warn(std::string);
    void error(std::string);
}
