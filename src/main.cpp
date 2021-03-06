import Lunar;

#include <vulkan/vulkan.hpp>

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

    Lunar::Renderer Renderer;
    Renderer.start();

    while (Window.active()) {
        Window.update();
        Renderer.draw();
    }

    Renderer.stop();
    Window.stop();

    glfwTerminate();
    return 0;
}
