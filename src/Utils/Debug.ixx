module;
#include <fmt/core.h>
#include <fmt/color.h>
#include <string>
#ifdef _WIN32
#include <Windows.h>
#endif
export module Lunar:Debug;

// Enable unix escape codes on capable Windows machines
void EnableVTMode()
{
#ifdef _WIN32
    static bool Enabled = true;
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
#endif
}

export namespace Lunar {
    template <typename S, typename... Args>
    void Print(const S& format_str, Args&&... args)
    {
#ifndef NDEBUG
        EnableVTMode();
        std::string Result = fmt::format(format_str, args...);
        fmt::print(
            fmt::fg(fmt::color::white),
            "[INFO] {}\n", Result
        );
#endif
    }

    template <typename S, typename... Args>
    void Warn(const S& format_str, Args&&... args)
    {
#ifndef NDEBUG
        EnableVTMode();
        std::string Result = fmt::format(format_str, args...);
        fmt::print(
            fmt::fg(fmt::color::yellow),
            "[WARNING] {}\n", Result
        );
#endif
    }

    template <typename S, typename... Args>
    void Error(const S& format_str, Args&&... args)
    {
#ifndef NDEBUG
        EnableVTMode();
        std::string Result = fmt::format(format_str, args...);
        fmt::print(
            fmt::fg(fmt::color::orange_red),
            "[ERROR] {}\n", Result
        );
#endif
    }

    void Exit()
    {
        fmt::print(
            fmt::fg(fmt::color::red),
            "EXIT CALLED."
        );
        exit(-1);
    }
}