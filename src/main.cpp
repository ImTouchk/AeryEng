import Lunar;

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <fmt/core.h>

int main()
{
    glfwInit();

    Lunar::WindowCreateInfo CreateInfo = {
        .title = "Aery Engine"
    };

    Lunar::Window Window;
    Window.start(CreateInfo);

    while (Window.active()) {
        Window.update();
    }

    Window.stop();

    glfwTerminate();
    return 0;
}
