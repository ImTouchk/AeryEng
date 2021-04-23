#include <GLFW/glfw3.h>

void StartWindow()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hi", NULL, NULL);
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}