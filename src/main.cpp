#include "aery.hpp"

void Start() {
    using namespace Aery;
    const WindowCreateInfo WCInfo = {
        .title = "Hello, world!",
        .width = 800,
        .height = 600,
        .flags = WINDOW_VSYNC | WINDOW_FULLSCREEN
    };

    Window GameWindow = {};
    if (!GameWindow.create(WCInfo)) {
        return;
    }

    VkRenderer GameRenderer = {};
    if (!GameRenderer.create(GameWindow)) {
        GameWindow.destroy();
        return;
    }

    VkObject* VertexObject;
    VkObjectCreateInfo ObjectInfo = {
        .vertices = {
            { { -0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
            { { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
            { {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },

            { {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
            { { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
            { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f } }
        },
    };
    GameRenderer.createObject(ObjectInfo, nullptr);

    while (GameWindow.active()) {
        GameWindow.update();
        GameRenderer.draw();
    }

    GameRenderer.destroy();
    GameWindow.destroy();
}

// Program entry point
#ifdef NDEBUG
#include <Windows.h>
int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine, int CmdShow) {
#else
int main() {
#endif
    Start();
    return 0;
}