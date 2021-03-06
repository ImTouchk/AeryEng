module;
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdlib>
module Lunar:Window;
import :Debug;

namespace {
    void OnResize(GLFWwindow* Handle, int Width, int Height)
    {
        Lunar::Window* Window = reinterpret_cast<Lunar::Window*>(
            glfwGetWindowUserPointer(Handle)
        );
    
    }

    void OnError(int Code, const char* Message)
    {
        Lunar::Error("<Window> Code {} - {}", Code, Message);
    }
}

namespace Lunar {
    bool Window::start(WindowCreateInfo& CreateInfo)
    {
        if (m_Active) {
            Warn("<Window> Start function called but window is already active.");
            return false;
        }

        glfwSetErrorCallback(OnError);

        glfwWindowHint(GLFW_RESIZABLE, CreateInfo.resizable);
        glfwWindowHint(GLFW_MAXIMIZED, CreateInfo.maximized);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Using Vulkan instead

        GLFWmonitor* Monitor = NULL;

        i32 Width = CreateInfo.width;
        i32 Height = CreateInfo.height;
        if (CreateInfo.fullscreen) {
            Monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* VideoMode = glfwGetVideoMode(Monitor);
            Width = VideoMode->width;
            Height = VideoMode->height;
        }

        m_Handle = glfwCreateWindow(
            Width.value(),
            Height.value(),
            CreateInfo.title,
            Monitor,
            NULL
        );

        if (m_Handle == NULL) {
            Error("<Window> Creation failed.");
            Exit();
            return false;
        }

        glfwSetWindowUserPointer(m_Handle, this);
        glfwSetFramebufferSizeCallback(m_Handle, OnResize);

        m_Active = true;
        m_Data = CreateInfo;

        return true;
    }

    void Window::stop()
    {
        if (!m_Active) {
            return;
        }
        glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
        m_Active = false;
    }

    void Window::update()
    {
        glfwPollEvents();
        m_Active = !glfwWindowShouldClose(m_Handle);
    }

    bool Window::active()
    {
        return m_Active;
    }
}

