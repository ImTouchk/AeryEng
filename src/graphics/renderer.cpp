#include "graphics/renderer.h"

#include "core/common.h"
#include "graphics/gl_renderer.h"

namespace Lunar
{
    Renderer* getRenderer()
    {
        static GLRenderer* renderer = new GLRenderer();
        return renderer;
    }
}
