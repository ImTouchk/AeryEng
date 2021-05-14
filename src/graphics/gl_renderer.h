#ifndef _LUNAR_GRAPHICS_GLRENDERER_H
#define _LUNAR_GRAPHICS_GLRENDERER_H

#include "core/common.h"
#include "graphics/renderer.h"

namespace Lunar
{
    class GLRenderer : public Renderer
    {
    public:
        GLRenderer(void* window);

        void stop() override;
        bool start() override;
        bool active() const override;
        void clear(fsize r, fsize g, fsize b, fsize a) override;
        void show() override;

    private:
        void* m_Window;
        bool  m_Active;
    };
}

#endif // _LUNAR_GRAPHICS_GLRENDERER_H
