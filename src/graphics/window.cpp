#include "graphics/window.h"

#include "graphics/renderer.h"
#include "gl_renderer.h"

#ifdef WIN32
#   include "dx11_renderer.h"
#endif

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
          m_Samples(info._samples),
          m_PreferredRenderer(info._renderer),
          m_StartMaximized(info.maximized),
          m_Resizable(info.resizable),
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

    void* Window::handle() const
    {
        return m_Handle;
    }

    Renderer* Window::getRenderer()
    {
        static Renderer* _ren = nullptr;

        auto& renderer = m_PreferredRenderer;
        if (renderer.compare("opengl") == 0)
            _ren = new GLRenderer(this);
#       ifdef WIN32
        else if (renderer.compare("dx11") == 0)
            _ren = new DX11Renderer(this);
#       endif
        else _ren = new GLRenderer(this);

        return _ren;
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

        if (m_PreferredRenderer.compare("opengl") == 0) {
            glfwWindowHint(GLFW_SAMPLES, m_Samples);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        } else {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }

        glfwWindowHint(GLFW_RESIZABLE, m_Resizable);
        glfwWindowHint(GLFW_MAXIMIZED, m_StartMaximized);

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

        glfwDestroyWindow(
            reinterpret_cast<GLFWwindow*>(m_Handle)
        );

        m_Handle = nullptr;
    }

    Window::createInfo Window::createInfo::fromFile(
        const std::string_view path, 
        const std::string_view title
    )
    {
        if (!std::filesystem::exists(path)) {
            Lunar::print(
                "Could not find config file '{}'. Loading default values for Window::createInfo.", 
                path
            );

            return Window::createInfo {
                .width      = 800,
                .height     = 600,
                .title      = title,
                .fullscreen = false,
                .resizable  = true,
                .maximized  = true,
                ._samples   = 4,
                ._renderer  = "opengl"
            };
        }

        auto  file       = toml::parse_file(path);
        usize width      = file["window"]["width"].value_or<usize>(800);
        usize height     = file["window"]["height"].value_or<usize>(600);
        bool  fullscreen = file["window"]["fullscreen"].value_or<bool>(false);
        bool  maximized  = file["window"]["maximized"].value_or<bool>(false);
        bool  resizable  = file["window"]["resizable"].value_or<bool>(true);
        usize samples    = file["opengl"]["samples"].value_or<usize>(2);
        auto  renderer   = file["window"]["renderer"].value_or<std::string>("opengl");

        Lunar::print("Loaded config file '{}'.", path);

        return Window::createInfo {
            .width      = width,
            .height     = height,
            .title      = title,
            .fullscreen = fullscreen,
            .resizable  = resizable,
            .maximized  = maximized,
            ._samples   = samples,
            ._renderer  = renderer,
        };
    }
}
