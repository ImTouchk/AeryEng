#include "aery.hpp"

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

    PushConstant MatrixBuffer = {};
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
    GameRenderer.bindPushConstant(VertexObject, &MatrixBuffer);

    mut_f32 Angle = 0.0f;
    mut_f32 OldTime = 0.0f;
    while (GameWindow.active()) {
        mut_f32 CurrentTime = static_cast<mut_f32>( glfwGetTime() );
        mut_f32 DeltaTime = CurrentTime - OldTime;
        OldTime = CurrentTime;

        auto Cursor = GameInput.getCursorPos();
        Cursor.first /= GameWindow.width();
        Cursor.second /= GameWindow.height();

        if (GameInput.isButton(MouseButton::e1)) Angle += 1.0f; 
        if (Angle >= 360.0f) Angle = 0.0f;

        MatrixBuffer.transform = glm::translate(glm::mat4(1.0f), { Cursor.first, Cursor.second, 0.6f });
        MatrixBuffer.transform = glm::rotate(MatrixBuffer.transform, glm::radians(Angle), { 1.0f, 1.0f, 1.0f });

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