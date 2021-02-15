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
            { {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
            { {  0.0f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
        },
        .indices = {
            0, 1, 2,
        },
        .shader = 0
    };
    GameRenderer.createObject(ObjectInfo, &VertexObject);

    PushConstant Constant = {};

    GameRenderer.bindPushConstant(VertexObject, &Constant);

    mut_f32 Angle = 0.0f;
    mut_f32 OldTime = 0.0f;
    while (GameWindow.active()) {
        mut_f32 CurrentTime = static_cast<mut_f32>( glfwGetTime() );
        mut_f32 DeltaTime = CurrentTime - OldTime;
        OldTime = CurrentTime;

        Angle += 1.0f; if (Angle >= 360.0f) Angle = 0.0f;
        Constant.transform = glm::rotate(glm::mat4(1.0f), glm::radians(Angle), { 1.0f, 0.0f, 0.0f });

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
    Start();
    glfwTerminate();
    return 0;
}