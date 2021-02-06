#pragma once

#include "utils/types.hpp"
#include <GLFW/glfw3.h>

namespace Aery { namespace Graphics {
    class VkRenderer;
    class GLRenderer;

    enum class WindowCreateFlags : mut_u16 {
        eVsync = 0,
        eFullscreen = 1 << 0,
        eResizable = 1 << 1,
        eMaximized = 1 << 2,
        eGLContext = 1 << 3,
    };

    WindowCreateFlags operator|(WindowCreateFlags a, WindowCreateFlags b) {
        return static_cast<WindowCreateFlags>(static_cast<mut_u16>(a) | static_cast<mut_u16>(b));
    }
    bool operator&(WindowCreateFlags a, WindowCreateFlags b) {
        return static_cast<bool>(static_cast<mut_u16>(a) & static_cast<mut_u16>(b));
    }

    enum class WindowRenderer : mut_u16 {
        eNone = 0,
        eOpenGL = 1 << 0,
        eVulkan = 1 << 1
    };

    struct WindowCreateInfo {
        const char* title = "Untitled Window";
        const WindowCreateFlags flags;
        const u16 width;
        const u16 height;
    };

    struct WindowInfo {
        GLFWwindow* handle;
        bool& glSurface;
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
        const u16 width() const;
        const u16 height() const;
        const bool active();

        void _onResize(u16, u16);
        void _onVulkanCreated(Graphics::VkRenderer&);
        void _onGLCreated(Graphics::GLRenderer&);

    private:
        bool m_Created;
        bool m_GLSurface;
        mutable bool m_Active;
        Graphics::VkRenderer* m_VkRenderer = nullptr;
        Graphics::GLRenderer* m_GLRenderer = nullptr;
        WindowRenderer m_Renderer = WindowRenderer::eNone;
        GLFWwindow* m_Handle;
        mut_u16 m_Height;
        mut_u16 m_Width;
        mut_u16 m_ID;
    };
}
}