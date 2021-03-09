#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>
#include <cassert>
#include <mutex>

#include "Debug.h"
#include "Types.h"
#include "Graphics/Renderer.h"

namespace {
    u32 Renderers = 0;
    std::mutex CountMutex;
}

namespace Lunar {
    void Renderer::start(Window& CreateInfo)
    {
        if(m_Active == true) {
            Lunar::Warn("Renderer> Start method was called but renderer is already active.");
            Lunar::Print("Renderer> Calling stop automatically...");
            stop();
        }

        {
            std::unique_lock<std::mutex> Lock(CountMutex);
            m_ID = Renderers++;
        }

        Lunar::PrintColor(fmt::color::light_sea_green);
        Lunar::Print("------- CREATING RENDERER {} -------", m_ID);
        Lunar::PrintColor(fmt::color::white);

        m_Window = &CreateInfo;
        m_Window->m_Renderer = this;

        CreatePermanentResources();
        CreateTemporaryResources();

        m_Active = true;
    }

    void Renderer::stop()
    {
        if(m_Active == false) {
            Lunar::Warn("Renderer> Stop method was called but renderer is already inactive.");
            return;
        }

        Lunar::PrintColor(fmt::color::light_coral);
        Lunar::Print("------- DESTROYING RENDERER {} -------", m_ID);
        Lunar::PrintColor(fmt::color::white);

        DestroyTemporaryResources();
        DestroyPermanentResources();
    }

    void Renderer::draw()
    {
        assert(m_Active == true);
    }

    bool Renderer::active() const
    {
        return true;
    }

    void Renderer::CreateTemporaryResources()
    {
        CreateSwapchain();
        CreateImageViews();
        CreateRenderPass();
        CreateFramebuffers();
    }

    void Renderer::DestroyTemporaryResources()
    {
        vkDeviceWaitIdle(m_Device);

        DestroyFramebuffers();
        DestroyRenderPass();
        DestroyImageViews();
        DestroySwapchain();
    }

    void Renderer::CreatePermanentResources()
    {
        vk::getInstance();
        SetupMessenger();
        CreateSurface();
        PickPhysicalDevice();
        CreateDevice();
        CreateAllocator();
    }

    void Renderer::DestroyPermanentResources()
    {
        DestroyAllocator();
        DestroyDevice();
        DestroySurface();
        DestroyMessenger();

        std::unique_lock<std::mutex> Lock(CountMutex);
        if(Renderers-- == 0) {
            vk::destroyInstance();
        }
    }

    void Renderer::OnResize()
    {
        Lunar::PrintColor(fmt::color::misty_rose);
        Lunar::Print("------- RENDERER {} RESIZE EVENT -------", m_ID);
        Lunar::PrintColor(fmt::color::white);
        DestroyTemporaryResources();
        CreateTemporaryResources();
    }
}