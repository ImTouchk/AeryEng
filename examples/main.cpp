#include "core/common.h"
#include "graphics/window.h"
#include "graphics/renderer.h"

#include <Windows.h>

int main()
{
    Lunar::Window window = {
        Lunar::Window::createInfo::fromFile(
            "config.toml",
            "Hello, world!"
        )
    };

    auto& renderer = *window.getRenderer();
    renderer.start();

    while (window.active()) {
        window.update();
        renderer.clear(1.0f, 0.0f, 0.0f, 1.0f);
        renderer.show();
    }

    return 1;
}