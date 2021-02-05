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
        WINDOW_GL_CONTEXT = 8,
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
        const u32 width;
        const u32 height;
    };

    class Window {
    public:
        Window(); ~Window();
        
        bool create(const WindowCreateInfo&&) = delete;
        bool create(const WindowCreateInfo&);
        bool recreate(const WindowCreateInfo&&) = delete;
        inline bool recreate(const WindowCreateInfo&);
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
    };
}
