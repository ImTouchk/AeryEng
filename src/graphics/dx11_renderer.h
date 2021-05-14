#ifndef _LUNAR_GRAPHICS_DX11RENDERER_H
#define _LUNAR_GRAPHICS_DX11RENDERER_H

#include "core/common.h"
#include "graphics/renderer.h"

#pragma comment (lib, "d3d11.lib")
#include <d3d11.h>

namespace Lunar
{
    class DX11Renderer : public Renderer
    {
    public:
        DX11Renderer(void* window);

        void stop() override;
        bool start() override;
        bool active() const override;

    private:
        bool                    m_Active;
        void*                   m_Window;
        IDXGISwapChain*         m_Swapchain;
        ID3D11Device*           m_Device;
        ID3D11DeviceContext*    m_DeviceCon;
        ID3D11RenderTargetView* m_BackBuffer;
    };
}

#endif // _LUNAR_GRAPHICS_DX11RENDERER_H
