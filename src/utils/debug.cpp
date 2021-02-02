#include "debug.hpp"
#include <fmt/color.h>
#include <fmt/core.h>
#include <fstream>
#include <string>

static void WriteToFile(std::string& Message) {
    std::fstream File = std::fstream("log.txt");
    if (!File.is_open()) {
        return;
    }
    File << Message;
    File.close();
}

namespace Aery {
    void log(std::string Message, fmt::color Color) {
        fmt::print(fmt::fg(Color), "[INFO] {}\n", Message);
        WriteToFile(Message);
    }

    void warn(std::string Message) {
        fmt::print(fmt::fg(fmt::color::yellow), "[WARNING] {}\n", Message);
        WriteToFile(Message);
    }

    void error(std::string Message) {
        fmt::print(fmt::fg(fmt::color::orange_red), "[ERROR] {}\n", Message);
        WriteToFile(Message);
    }
}
