#include "graphics/gl_renderer.h"

#include "core/common.h"
#include "graphics/window.h"
#include "graphics/renderer.h"

namespace Lunar
{
    bool GLRenderer::start(Window* window)
    {
        return true;
    }

    void GLRenderer::stop()
    {

    }

    bool GLRenderer::active() const
    {
        return true;
    }
}
