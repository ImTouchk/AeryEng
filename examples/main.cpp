#include "core/common.h"
#include "graphics/window.h"
#include "graphics/renderer.h"

int main()
{
    Lunar::Window window = {
        Lunar::Window::createInfo::fromFile(
            "config.toml",
            "Hello, world!"
        )
    };

    Lunar::Renderer* renderer = Lunar::getRenderer();

    while (window.active()) {
        window.update();
    }

    return 1;
}