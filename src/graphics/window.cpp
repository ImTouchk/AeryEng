#include "graphics/window.h"

#include <toml++/toml.h>
#include <GLFW/glfw3.h>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <string>

namespace
{
    constexpr isize ERROR_GLFW_INIT          = 100;
    constexpr isize ERROR_GLFW_WINDOW_CREATE = 101;
    isize           _WINDOW_COUNT   = 0;
}

namespace Lunar
{
    Window::Window(createInfo&& info)
        : m_Width(info.width),
          m_Height(info.height),
          m_Title(info.title),
          m_Fullscreen(info.fullscreen),
          m_Handle(nullptr)
    {
        isize result;
        if (_WINDOW_COUNT++ == 0) {
            result = glfwInit();
            if (result == GLFW_FALSE)
                exit(ERROR_GLFW_INIT);
        }

        start();
    }

    Window::~Window()
    {
        stop();

        if (--_WINDOW_COUNT == 0) {
            glfwTerminate();
        }
    }

    bool Window::active() const
    {
        return !glfwWindowShouldClose(
            reinterpret_cast<GLFWwindow*>(m_Handle)
        );
    }

    void Window::start()
    {
        if (m_Handle != nullptr) {
            Lunar::error("Window::start() called but window already is created.");
            return;
        }

        GLFWmonitor* monitor = nullptr;
        if (m_Fullscreen == true) {
            const GLFWvidmode* mode;
            monitor  = glfwGetPrimaryMonitor();
            mode     = glfwGetVideoMode(monitor);
            m_Width  = mode->width;
            m_Height = mode->height;
        }

        m_Handle = reinterpret_cast<void*>(
            glfwCreateWindow(
                m_Width, 
                m_Height, 
                m_Title.data(), 
                monitor, 
                nullptr
            )
        );

        if (m_Handle == nullptr) {
            const char* errorString;
            glfwGetError(&errorString);

            Lunar::error("Window::start() failed.");
            Lunar::error("GLFW -> {}", errorString);
            
            exit(ERROR_GLFW_WINDOW_CREATE);
        }
    }

    void Window::update()
    {
        glfwPollEvents();
    }

    void Window::stop()
    {
        if (m_Handle == nullptr)
            return;

        glfwSetWindowShouldClose(
            reinterpret_cast<GLFWwindow*>(m_Handle),
            GLFW_TRUE
        );
    }

    Window::createInfo Window::createInfo::fromFile(
        const std::string_view path, 
        const std::string_view title
    )
    {
        if (!std::filesystem::exists(path)) {
            return Window::createInfo {
                .width      = 800,
                .height     = 600,
                .title      = title,
                .fullscreen = false,
            };
        }

        auto  file       = toml::parse_file(path);
        usize width      = file["window"]["width"].value_or<usize>(800);
        usize height     = file["window"]["height"].value_or<usize>(600);
        bool  fullscreen = file["window"]["fullscreen"].value_or<bool>(false);

        return Window::createInfo {
            .width      = width,
            .height     = height,
            .title      = title,
            .fullscreen = fullscreen,
        };
    }
}
