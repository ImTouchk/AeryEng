#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Debug.h"
#include "Types.h"
#include "Graphics/Renderer.h"
#include "Graphics/Window.h"

namespace {
    void OnError(int Code, const char* Message)
    {
        Lunar::Error("GLFW> {}", Message);
    }
}

namespace Lunar {
    void _OnResize(GLFWwindow* Handle, int Width, int Height)
    {
        Lunar::Window* Current = reinterpret_cast<Lunar::Window*>(
            glfwGetWindowUserPointer(Handle)
        );

        /* 
            I am too lazy to replace this pointer trick (which is
            probably entering UB territory) with an actual solution
            and it amuzes me so I'm going to keep it for now :D
        */

        struct WindowInternal {
            GLFWwindow* _window;
            bool _active;
            u32 _width;
            u32 _height;
            Lunar::Renderer* _renderer;
        };

        static_assert(sizeof(WindowInternal) == sizeof(Lunar::Window));

        /* std::bit_cast is not supported yet on GCC */

#   ifdef _WIN32
        WindowInternal* Internal = std::bit_cast<WindowInternal*>(Current);
#   else
        WindowInternal* Internal = reinterpret_cast<WindowInternal*>(
            reinterpret_cast<void*>(Current)
        );
#   endif

        Internal->_width = static_cast<u32>(Width);
        Internal->_height = static_cast<u32>(Height);
        //Internal->_renderer->OnResize();

        Lunar::Print("Window> Resize event {{ width: {}, height: {} }}", Internal->_width, Internal->_height);
    }

    bool Window::start(const WindowCreateInfo& CreateInfo)
    {
        if(m_Active == true) {
            Lunar::Warn("Window> Start method was called but window is already active.");
            Lunar::Print("Window> Calling stop automatically...");
            stop();
        }

        glfwSetErrorCallback(OnError);
        glfwWindowHint(GLFW_RESIZABLE, CreateInfo.resizable);
        glfwWindowHint(GLFW_MAXIMIZED, CreateInfo.maximized);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        GLFWmonitor* Monitor = NULL;
        
        u32 Width = CreateInfo.width;
        u32 Height = CreateInfo.height;

        if(CreateInfo.fullscreen) {
            Monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* VideoMode = glfwGetVideoMode(Monitor);
            Width = VideoMode->width;
            Height = VideoMode->height;
        }

        m_Handle = glfwCreateWindow(
            Width,
            Height,
            CreateInfo.title,
            Monitor,
            NULL
        );

        if(m_Handle == NULL) {
            Lunar::Error("Window> Creation failed.");
            Lunar::Exit();
            return false;
        }

        glfwSetWindowUserPointer(m_Handle, this);
        glfwSetFramebufferSizeCallback(m_Handle, _OnResize);

        Lunar::Print("Window> Creation successful.");
        m_Active = true;
        return true;
    }

    void Window::stop()
    {
        if(!m_Active) {
            Lunar::Warn("Window> Stop method was called but window is already inactive.");
            return;
        }

        glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
    }

    bool Window::active() const
    {
        return m_Active;
    }

    void Window::update()
    {
        glfwPollEvents();
        m_Active = !glfwWindowShouldClose(m_Handle);
    }

    u32 Window::width() const
    {
        return m_Width;
    }

    u32 Window::height() const
    {
        return m_Height;
    }
}