#ifndef LUNAR_WINDOW_H
#define LUNAR_WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Types.h"

namespace Lunar {
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
    private:
        GLFWwindow* m_Handle;
        bool m_Active;
        u32 height = 0;
        u32 width = 0;
    };
}

#endif