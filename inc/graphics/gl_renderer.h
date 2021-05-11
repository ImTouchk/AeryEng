#ifndef _LUNAR_GRAPHICS_GLRENDERER_H
#define _LUNAR_GRAPHICS_GLRENDERER_H

#include "core/common.h"
#include "graphics/renderer.h"

namespace Lunar
{
    class GLRenderer : public Renderer
    {
    public:
        void stop() override;
        bool start(Window* window) override;
        bool active() const override;
    };
}

#endif // _LUNAR_GRAPHICS_GLRENDERER_H
