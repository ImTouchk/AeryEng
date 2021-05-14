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

    auto* renderer = window.getRenderer();
    renderer->start();

    while (window.active()) {
        renderer->clear(1, 0, 0, 1);
        renderer->show();

        window.update();
    }

    return 1;
}