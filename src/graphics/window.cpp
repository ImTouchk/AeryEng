#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/gl_renderer.hpp"
#include "graphics/vk_renderer.hpp"
#include "graphics/window.hpp"
#include <fmt/color.h>
#include <fmt/core.h>
#include <vector>
#include <mutex>

using namespace std;

namespace {
    Aery::mut_u16 Index = 1;
    mutex Mutex = {};
}

static void OnWindowResize(GLFWwindow* Window, int Width, int Height) {
    Aery::Graphics::Window* Current = reinterpret_cast<Aery::Graphics::Window*>(glfwGetWindowUserPointer(Window));
    Current->_onResize(static_cast<Aery::mut_u16>(Width), static_cast<Aery::mut_u16>(Height));
}

namespace Aery { namespace Graphics {
    Window::Window() : m_ID(Index) {
        Mutex.lock(); Index++; Mutex.unlock();
        m_Active = false;
        m_Created = false;
    }

    Window::~Window() {
        if (m_Created) {
            destroy();
        }
    }

    bool Window::recreate(const WindowCreateInfo& Info) {
        destroy();
        return create(Info);
    }

    bool Window::create(const WindowCreateInfo& Info) {
        if (m_Active) {
            Aery::error(fmt::format("<Window::create> ID {} is already active.", m_ID));
            return false;
        }

        glfwWindowHint(GLFW_RESIZABLE, Info.flags & WindowCreateFlags::eResizable);
        glfwWindowHint(GLFW_MAXIMIZED, Info.flags & WindowCreateFlags::eMaximized);

        glfwWindowHint(GLFW_CLIENT_API, (Info.flags & WindowCreateFlags::eGLContext) ? GLFW_OPENGL_API : GLFW_NO_API);
        if (Info.flags & WindowCreateFlags::eGLContext) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            m_GLSurface = true;
        }

        GLFWmonitor* Monitor = nullptr;
        int Width   = Info.width,
            Height  = Info.height;
        if (Info.flags & WindowCreateFlags::eFullscreen) {
            Monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* Mode = glfwGetVideoMode(Monitor);
            Width = Mode->width; Height = Mode->height;
        }

        m_Handle = glfwCreateWindow(
            static_cast<int>(Width), 
            static_cast<int>(Height), 
            Info.title, 
            Monitor, 
            nullptr
        );
        m_Width = Info.width; m_Height = Info.height;

        if (m_Handle == NULL) {
            Aery::error(fmt::format("<Window::create> ID {} creation failed.", m_ID));
            return false;
        }
        glfwSetWindowUserPointer(m_Handle, this);
        glfwSetFramebufferSizeCallback(m_Handle, (GLFWframebuffersizefun)OnWindowResize);
        
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
        if (m_GLSurface) {
            glfwSwapBuffers(m_Handle);
        }
    }

    WindowInfo Window::info() {
        WindowInfo Info = {
            m_Handle,
            m_GLSurface,
        };
        return Info;
    }

    const u16 Window::width() const {
        return m_Width;
    }

    const u16 Window::height() const {
        return m_Height;
    }

    const bool Window::active() {
        m_Active = !glfwWindowShouldClose(m_Handle);
        return m_Active;
    }

    void Window::_onResize(u16 Width, u16 Height) {
        m_Width = Width; m_Height = Height;
        switch (m_Renderer) {
            case WindowRenderer::eOpenGL: m_GLRenderer->_onResize();
            case WindowRenderer::eVulkan: m_VkRenderer->_onResize();
            default: break;
        }
    }

    void Window::_onVulkanCreated(Graphics::VkRenderer& Renderer) {
        m_Renderer = WindowRenderer::eVulkan;
        m_VkRenderer = &Renderer;
    }

    void Window::_onGLCreated(Graphics::GLRenderer& Renderer) {
        m_Renderer = WindowRenderer::eOpenGL;
        m_GLRenderer = &Renderer;
    }
}
}