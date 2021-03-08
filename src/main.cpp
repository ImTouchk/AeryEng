#include "Graphics/Window.h"
#include "Graphics/Renderer.h"

int main()
{
    glfwInit();

    Lunar::WindowCreateInfo WindowCreateInfo = { .maximized = false };
    Lunar::Window Window;
    Lunar::Renderer Renderer;

    Window.start(WindowCreateInfo);
    Renderer.start(Window);
    while(Window.active()) {
        Window.update();
    }
    Renderer.stop();
    Window.stop();

    glfwTerminate();
    return 0;
}