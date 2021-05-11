#ifndef _LUNAR_GRAPHICS_RENDERER_H
#define _LUNAR_GRAPHICS_RENDERER_H

#include "core/common.h"
#include "graphics/window.h"

namespace Lunar
{
    class Renderer
    {
    public:
        virtual ~Renderer() {}
        virtual bool start(Window& window) = 0;
        virtual void stop()  = 0;
        virtual bool active() const = 0;
    };

    /* picks a graphics backend */
    Renderer* getRenderer();
}

#endif // _LUNAR_GRAPHICS_RENDERER_H
