#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vk_renderer.hpp"
#include "graphics/window.hpp"
#include <fmt/color.h>
#include <fmt/core.h>
#include <vector>
#include <mutex>

using namespace std;

static vector<Aery::Window*> Windows = {};
static Aery::mut_u32 Index = 0;
static mutex Mutex = {};

static void OnWindowResize(GLFWwindow* Window, int Width, int Height) {
    Aery::Window* Current = nullptr;
    for (Aery::Window* Pointer : Windows) {
        if (Pointer->info().handle == Window) {
            Current = Pointer;
            break;
        }
    }
    if (Current == nullptr) {
        return;
    }
    Current->_onResize((Aery::u32)Width, (Aery::u32)Height);
}

namespace Aery {
    Window::Window() : m_ID(Index) {
        Mutex.lock();
        Windows.push_back(this);
        Index++;
        Mutex.unlock();

        m_Active = false;
        m_Created = false;
    }

    Window::~Window() {
        Mutex.lock();
        Windows.erase(
            Windows.begin() + m_ID
        );
        Mutex.unlock();
        if (m_Created) {
            destroy();
        }
    }

    bool Window::create(const WindowCreateInfo& Info) {
        if (m_Active) {
            Aery::error(fmt::format("<Window::create> ID {} is already active.", m_ID));
            return false;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, Info.flags & WINDOW_RESIZABLE);
        glfwWindowHint(GLFW_MAXIMIZED, Info.flags & WINDOW_MAXIMIZED);
        m_VSync = Info.flags & WINDOW_VSYNC;

        GLFWmonitor* Monitor = nullptr;
        int Width   = Info.width,
            Height  = Info.height;
        if (Info.flags & WINDOW_FULLSCREEN) {
            Monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* Mode = glfwGetVideoMode(Monitor);
            Width = Mode->width; Height = Mode->height;
        }

        m_Handle = glfwCreateWindow(Width, Height, Info.title, Monitor, nullptr);
        m_Width = Info.width; m_Height = Info.height;
        glfwSetFramebufferSizeCallback(m_Handle, (GLFWframebuffersizefun)OnWindowResize);

        if (m_Handle == NULL) {
            Aery::error(fmt::format("<Window::create> ID {} creation failed.", m_ID));
            return false;
        }
        
        Aery::log(fmt::format("<Window::create> ID {} was started.", m_ID), fmt::color::light_green);
        glfwMakeContextCurrent(m_Handle);
        m_Active = true;
        m_Created = true;
        return m_Active;
    }

    void Window::destroy() {
        if (!m_Created) {
            Aery::warn(fmt::format("<Window::destroy> ID {} is already destroyed.", m_ID));
            return;
        }

        m_Created = false;
        glfwDestroyWindow(m_Handle);
        Aery::log(fmt::format("<Window::destroy> ID {} was destroyed.", m_ID));
    }

    void Window::update() const {
        glfwPollEvents();
    }

    WindowInfo Window::info() {
        WindowInfo Info = {
            m_Handle,
            m_GLSurface,
            m_VSync,
            m_Width,
            m_Height
        };
        return Info;
    }

    const u32 Window::width() const {
        return m_Width;
    }

    const u32 Window::height() const {
        return m_Height;
    }

    const bool Window::active() {
        m_Active = !glfwWindowShouldClose(m_Handle);
        return m_Active;
    }

    void Window::_onResize(const u32 Width, const u32 Height) {
        m_Width = Width; m_Height = Height;
        if (m_Renderer != nullptr) {
            m_Renderer->_onResize();
        }
    }

    void Window::_onRendererCreated(VkRenderer& Renderer) {
        m_Renderer = &Renderer;
    }
}