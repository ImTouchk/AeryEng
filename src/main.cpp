#include "aery.hpp"

void Start() {
    using namespace Aery;
    WindowCreateInfo WCInfo = {
        .title = "Hello, world!",
        .width = 800,
        .height = 600,
        .flags = WINDOW_VSYNC | WINDOW_RESIZABLE
    };

    Window GameWindow = {};
    if (!GameWindow.create(WCInfo)) {
        return;
    }

    VkRendererCreateInfo VRInfo = {
        .window = &GameWindow,
        .render_mode = VK_RENDERER_TRIPLE_BUFFERING,
    };

    VkRenderer GameRenderer = {};
    if (!GameRenderer.create(VRInfo)) {
        GameWindow.destroy();
        return;
    }

    PVkObject VertexObject;
    VkObjectCreateInfo ObjectInfo = {
        .vertices = {
            { {  1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
            { { -1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
            { {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
        },
        .indices = {
            0, 1, 2,
        }
    };
    GameRenderer.createObject(ObjectInfo, &VertexObject);

    while (GameWindow.active()) {
        GameWindow.update();
        GameRenderer.draw();
    }

    GameRenderer.destroy();
    GameWindow.destroy();
}

// Program entry point
#include <GLFW/glfw3.h>
#ifdef NDEBUG
#include <Windows.h>
int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine, int CmdShow) {
#else
int main() {
#endif
    glfwInit();
    Start();
    glfwTerminate();
    return 0;
}