#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Types.h"
#include "Debug.h"

#include "Graphics/Window.h"

namespace {
    void OnResize(GLFWwindow* Handle, int Width, int Height)
    {
        Lunar::PrintTitle("WINDOW");
        Lunar::Print("Resize event!");
        Lunar::PrintTitle("INFO");
    }

    void OnError(int Code, const char* Message)
    {
        Lunar::Error("[GLFW] {}", Message);
    }
}

namespace Lunar {
    bool Window::start(const WindowCreateInfo& CreateInfo)
    {
        Lunar::PrintTitle("WINDOW");
        if(m_Active == true) {
            Lunar::Warn("Start method was called but window is already active.");
            Lunar::Print("Calling stop automatically...");
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
            Lunar::Error("Creation failed.");
            Lunar::Exit();
            return false;
        }

        glfwSetWindowUserPointer(m_Handle, this);
        glfwSetFramebufferSizeCallback(m_Handle, OnResize);

        Lunar::Print("Creation successful.");
        m_Active = true;
        return true;
    }

    void Window::stop()
    {
        if(!m_Active) {
            Lunar::Warn("Stop method was called but window is already inactive.");
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
}