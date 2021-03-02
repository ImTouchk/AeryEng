module;
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
export module Lunar:Window;
import :Numbers;

export namespace Lunar {
    struct WindowCreateInfo {
        bool vsync = false;
        bool resizable = true;
        bool maximized = true;
        bool fullscreen = false;

        const char* title = "Untitled Window";

        i32 width = 800;
        i32 height = 600;
    };

    class Window {
    public:
        Window() = default;
        ~Window() = default;

        bool start(WindowCreateInfo& CreateInfo);
        void stop();

        bool active();
        void update();

    private:
        friend void OnResize(GLFWwindow* Handle, int Width, int Height);

        using WindowData = WindowCreateInfo;

        GLFWwindow* m_Handle;
        WindowData m_Data;
        bool m_Active = false;
    };
}
