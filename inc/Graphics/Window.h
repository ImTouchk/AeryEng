#ifndef LUNAR_WINDOW_H
#define LUNAR_WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Types.h"

namespace Lunar {
    class Renderer;

    struct WindowCreateInfo {
        const char* title = "Untitled Window";
        const u32 height = 600;
        const u32 width = 800;
        const bool resizable = true;
        const bool maximized = true;
        const bool fullscreen = false;
    };

    class Window { 
    public:
        Window() = default;
        ~Window() = default;

        bool start(const WindowCreateInfo& CreateInfo);
        void stop();

        bool active() const;
        void update();

        u32 width() const;
        u32 height() const;
    private:
        friend void _OnResize(GLFWwindow* Handle, int Width, int Height);
        friend class Renderer;

        GLFWwindow* m_Handle = NULL;
        bool m_Active        = false;
        u32 m_Height = 0;
        u32 m_Width  = 0;
        Renderer* m_Renderer = NULL;
    };

}

#endif