#ifndef _LUNAR_GRAPHICS_RENDERER_H
#define _LUNAR_GRAPHICS_RENDERER_H

#include "core/common.h"

namespace Lunar
{
    class Renderer
    {
    public:
        virtual ~Renderer() {}
        virtual bool start() = 0;
        virtual void stop()  = 0;
        virtual bool active() const = 0;
        virtual void clear(fsize r, fsize g, fsize b, fsize a) = 0;
        virtual void show() = 0;
    };
}

#endif // _LUNAR_GRAPHICS_RENDERER_H
