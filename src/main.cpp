#include <iostream>

#include <GLFW/glfw3.h>

void test()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(
        800, 600, "Hello, world!", nullptr, nullptr
    );

    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}