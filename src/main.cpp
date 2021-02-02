#include "aery.hpp"

int main() {
    using namespace Aery;
    const WindowCreateInfo WCInfo = {
        .title = "Hello, world!",
        .width = 800,
        .height = 600,
        .flags = WINDOW_VSYNC
    };

    Window GameWindow = {};
    if (!GameWindow.create(WCInfo)) {
        return 0;
    }

    VkRenderer GameRenderer = {};
    if (!GameRenderer.create(GameWindow)) {
        GameWindow.destroy();
        return 0;
    }

    VkShader Default = {};
    GameRenderer.createDefaultShader(&Default);
    GameRenderer.createDefaultShader(nullptr);
    GameRenderer.createDefaultShader(nullptr);
    GameRenderer.createDefaultShader(nullptr);

    while (GameWindow.active()) {
        GameWindow.update();
        GameRenderer.draw();
    }

    GameRenderer.destroy();
    GameWindow.destroy();
    return 0;
}