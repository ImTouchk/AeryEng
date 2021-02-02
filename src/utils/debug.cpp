#include "debug.hpp"
#include <fmt/color.h>
#include <fmt/core.h>
#include <fstream>
#include <string>
#include <Windows.h>

static void WriteToFile(std::string& Message) {
    std::fstream File = std::fstream("log.txt");
    if (!File.is_open()) {
        return;
    }
    File << Message;
    File.close();
}

// Windows-specific : enable ANSI escape codes
static void EnableVTMode() {
    static bool Enabled = false;
    if (!Enabled) {
        HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
        if (Console == INVALID_HANDLE_VALUE) {
            return;
        }
        DWORD Mode = 0;
        if (!GetConsoleMode(Console, &Mode)) {
            return;
        }
        Mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(Console, Mode)) {
            return;
        }
        Enabled = true;
    }
}

namespace Aery {
    void log(std::string Message, fmt::color Color) {
#ifndef NDEBUG
        EnableVTMode();
        fmt::print(fmt::fg(Color), "[INFO] {}\n", Message);
        WriteToFile(Message);
#endif
    }

    void warn(std::string Message) {
#ifndef NDEBUG
        EnableVTMode();
        fmt::print(fmt::fg(fmt::color::yellow), "[WARNING] {}\n", Message);
        WriteToFile(Message);
#endif
    }

    void error(std::string Message) {
#ifndef NDEBUG
        EnableVTMode();
        fmt::print(fmt::fg(fmt::color::orange_red), "[ERROR] {}\n", Message);
        WriteToFile(Message);
#endif
    }
}
