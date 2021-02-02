#pragma once

#include "utils/types.hpp"
#include <GLFW/glfw3.h>

namespace Aery {
    class VkRenderer;

    enum WindowCreateFlags {
        WINDOW_VSYNC = 1,
        WINDOW_FULLSCREEN = 2,
        WINDOW_RESIZABLE = 4,
        WINDOW_MAXIMIZED = 6,
    };

    struct WindowCreateInfo {
        const char* title = "Untitled Window";
        const u32 width;
        const u32 height;
        const u16 flags;
    };

    struct WindowInfo {
        GLFWwindow* handle;
        bool& glSurface;
        bool vsync;
        const u32 width;
        const u32 height;
    };

    class Window {
    public:
        Window(); ~Window();
        
        bool create(const WindowCreateInfo&&) = delete;
        bool create(const WindowCreateInfo&);
        void destroy();
        void update() const;

        WindowInfo info();
        const u32 width() const;
        const u32 height() const;
        const bool active();

        void _onResize(const u32, const u32);
        void _onRendererCreated(VkRenderer&);

    private:
        u32 m_ID;
        bool m_Created;
        mutable bool m_Active;
        VkRenderer* m_Renderer;
        GLFWwindow* m_Handle;
        mut_u32 m_Height;
        mut_u32 m_Width;
        bool m_GLSurface;
        bool m_VSync;
    };
}
