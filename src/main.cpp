import Lunar;

#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <fmt/core.h>

#include <thread>

void program_start()
{
    Lunar::WindowCreateInfo CreateInfo = {
        .title = "Aery Engine"
    };

    Lunar::Window Window;
    Window.start(CreateInfo);

    Lunar::Renderer Renderer;
    Renderer.start(Window);

    while (Window.active()) {
        Window.update();
        Renderer.draw();
    }

    Renderer.stop();
    Window.stop();
    return;
}

int main()
{
    glfwInit();

    std::thread Second = std::thread(program_start);

    program_start();
        
    Second.join();

    glfwTerminate();
    return 0;
}
