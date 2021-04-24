#include "Lunar.hpp"

int main()
{
    Lunar::window window(
        Lunar::window::createInfo{
            .vsync       = false,
            .resizable   = true,
            .maximized   = false,
            .fullscreen  = false,
            .title       = "Example App",
            .width       = 800,
            .height      = 600,
        }
    );

    Lunar::renderer renderer(
        Lunar::renderer::createInfo{
            .target = window,
            .vsync  = false
        }
    );

    while(window.isActive()) {
        window.update();
    }
    return 0;
}
