#include "core/common.h"
#include "graphics/window.h"

int main()
{
    Lunar::Window window = {
        Lunar::Window::createInfo::fromFile(
            "config.toml",
            "Hello, world!"
        )
    };

    while (window.active()) {
        window.update();
    }

    return 1;
}