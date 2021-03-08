#include "Graphics/Window.h"
#include "Graphics/Renderer.h"

int main()
{
    glfwInit();

    Lunar::WindowCreateInfo WindowCreateInfo;
    Lunar::Window Window;

    Window.start(WindowCreateInfo);
    while(Window.active()) {

        Window.update();
    }
    Window.stop();

    glfwTerminate();
    return 0;
}