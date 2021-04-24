#include "graphics/window.hpp"
#include "graphics/renderer.hpp"

namespace Lunar {
    renderer::renderer(createInfo&& info)
    {
        createBoilerplate();
    }

    renderer::~renderer()
    {
        destroyBoilerplate();
    }
}
