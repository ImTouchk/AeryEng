#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdlib>

#include "types.hpp"
#include "graphics/window.hpp"

namespace {
    bool GLFW_INITIALIZED  = false;
    isize REFERENCE_COUNT  = 0; 

    void incrementCounter()
    {
        REFERENCE_COUNT++;

        if(!GLFW_INITIALIZED) {
            isize result = glfwInit();
            if(!result)
                exit(-1);

            GLFW_INITIALIZED = true;
        }
    }

    void decrementCounter()
    {
        REFERENCE_COUNT--;
        if(REFERENCE_COUNT == 0) {
            glfwTerminate();
            GLFW_INITIALIZED = false;
        }
    }
}

namespace Lunar {
    window::window(const createInfo&& info)
    {
        incrementCounter();

        GLFWmonitor* monitor = nullptr;
        m_Info        = info;
        if(m_Info.fullscreen) {
            monitor                 = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            m_Info.width  = mode->width;
            m_Info.height = mode->height;
        } else {
            glfwWindowHint(GLFW_RESIZABLE, m_Info.resizable);
            glfwWindowHint(GLFW_MAXIMIZED, m_Info.maximized);
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Handle = reinterpret_cast<isize*>(glfwCreateWindow(
            m_Info.width,
            m_Info.height,
            m_Info.title,
            monitor,
            nullptr
        ));

        if(m_Handle == nullptr) {
            exit(-1);
        }
    }

    window::~window()
    {
        glfwDestroyWindow(
            reinterpret_cast<GLFWwindow*>(m_Handle)
        );
        decrementCounter();
    }

    isize* window::handle() const
    {
        return m_Handle;
    }

    bool window::isActive() const
    {
        return !glfwWindowShouldClose(
            reinterpret_cast<GLFWwindow*>(m_Handle)
        );
    }

    void window::update()
    {
        glfwPollEvents();
    }
}
