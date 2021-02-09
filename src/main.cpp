#include "aery.hpp"
#include "utils/debug.hpp"
#include <fmt/core.h>
#include <thread>

void Start() {
    using namespace Aery;
    using namespace Aery::Graphics;
    WindowCreateInfo WCInfo = {
        .title = "Hello, world!",
        .flags = WindowCreateFlags::eResizable,
        .width = 800,
        .height = 600,
    };

    Window GameWindow = {};
    if (!GameWindow.create(WCInfo)) {
        return;
    }

    Input GameInput = {};
    GameInput._onWindowCreated(GameWindow);

    RendererCreateInfo RdInfo = {
        .present_mode = PresentMode::eVsync,
        .window = &GameWindow,
    };

    VkRenderer GameRenderer = {};
    if (!GameRenderer.create(RdInfo)) {
        GameWindow.destroy();
        return;
    }

    PObject VertexObject;
    ObjectCreateInfo ObjectInfo = {
        .vertices = {
            { { -0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
            { {  0.0f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
            { {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
            { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f } }
        },
        .indices = {
            0, 1, 2,
            2, 1, 3
        },
        .shader = 0
    };
    GameRenderer.createObject(ObjectInfo, &VertexObject);

    PushConstant Constant = {};
    Constant.transform = mat4(1.0f);

    GameRenderer.bindPushConstant(VertexObject, &Constant);

    mut_f32 OldTime = 0.0f;
    while (GameWindow.active()) {
        mut_f32 CurrentTime = static_cast<mut_f32>( glfwGetTime() );
        mut_f32 DeltaTime = CurrentTime - OldTime;
        OldTime = CurrentTime;

        GameRenderer.draw();
        GameWindow.update();
        
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
    std::thread Secondary = std::thread(Start);
    Secondary.join();
    glfwTerminate();
    return 0;
}