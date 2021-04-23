#include "Lunar.hpp"

#include <thread>

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

    while(window.isActive()) {
        window.update();
    }
    return 0;
}
