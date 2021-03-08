#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>

int main()
{
    if (!glfwInit()) {
        return -1;
    }
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* Window = glfwCreateWindow(800, 600, "Hello, world!", NULL, NULL);
    while(!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
    }
    glfwDestroyWindow(Window);

    glfwTerminate();
    return 0;
}