#include "dx11_renderer.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "core/common.h"
#include "graphics/window.h"

namespace
{
    constexpr isize ERROR_DX_DEVICE_FAIL = 300;
}

namespace Lunar
{
    DX11Renderer::DX11Renderer(void* window)
        : m_Active(false),
          m_Swapchain(nullptr),
          m_Device(nullptr),
          m_DeviceCon(nullptr),
          m_BackBuffer(nullptr),
          m_Window(window)
    {
    }

    bool DX11Renderer::start()
    {
        GLFWwindow* window = reinterpret_cast<GLFWwindow*>(
            ((Window*)m_Window)->handle()
        );
        HWND winHandle = glfwGetWin32Window(window);

        DXGI_SWAP_CHAIN_DESC swapDesc = {
            .BufferDesc = {
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            },
            .SampleDesc   = 2,
            .BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount  = 1,
            .OutputWindow = winHandle,
            .Windowed     = TRUE,
        };

        D3D_FEATURE_LEVEL levels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
        };

        D3D_FEATURE_LEVEL usedLevel;

        HRESULT result;
        result = D3D11CreateDeviceAndSwapChain(
            NULL,
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            NULL,
            levels,
            2,
            D3D11_SDK_VERSION,
            &swapDesc,
            &m_Swapchain,
            &m_Device,
            &usedLevel,
            &m_DeviceCon
        );

        if (result != S_OK) {
            Lunar::error("Couldn't initialize the LegacyDX renderer.");
            exit(ERROR_DX_DEVICE_FAIL);
        }
        
        ID3D11Texture2D* backBuffer;
        m_Swapchain->GetBuffer(
            0, 
            __uuidof(ID3D11Texture2D), 
            (LPVOID*)&backBuffer
        );

        m_Device->CreateRenderTargetView(
            backBuffer,
            NULL,
            &m_BackBuffer
        );

        backBuffer->Release();

        m_DeviceCon->OMSetRenderTargets(
            1, 
            &m_BackBuffer, 
            NULL
        );

        D3D11_VIEWPORT viewport = {
            .TopLeftX = 0,
            .TopLeftY = 0,
            .Width    = 800,
            .Height   = 600,
        };
        m_DeviceCon->RSSetViewports(1, &viewport);

        Lunar::print("LegacyDX renderer started.");
        return true;
    }

    void DX11Renderer::stop()
    {
        m_Swapchain->Release();
        m_BackBuffer->Release();
        m_Device->Release();
        m_DeviceCon->Release();
    }

    bool DX11Renderer::active() const
    {
        return true;
    }

    void DX11Renderer::clear(fsize r, fsize g, fsize b, fsize a)
    {
        float clearColor[4] = { 
            static_cast<float>(r), 
            static_cast<float>(g),
            static_cast<float>(b),
            static_cast<float>(a) 
        };

        m_DeviceCon->ClearRenderTargetView(
            m_BackBuffer,
            clearColor
        );
    }
}
