#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>
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
        auto CreatePermanentResources = [&]() {
            vk::getInstance();
            SetupMessenger();
            CreateSurface();
            PickPhysicalDevice();
            CreateDevice();
            CreateAllocator();
        };

        {
            std::unique_lock<std::mutex> Lock(CountMutex);
            m_ID = Renderers++;
        }

        Lunar::PrintColor(fmt::color::light_sea_green);
        Lunar::Print("------- CREATING RENDERER {} -------", m_ID);
        Lunar::PrintColor(fmt::color::white);

        m_Window = &CreateInfo;
        CreatePermanentResources();
    }

    void Renderer::stop()
    {
        Lunar::PrintColor(fmt::color::light_coral);
        Lunar::Print("------- DESTROYING RENDERER {} -------", m_ID);
        Lunar::PrintColor(fmt::color::white);

        auto DestroyPermanentResources = [&]() {
            DestroyAllocator();
            DestroyDevice();
            DestroySurface();
            DestroyMessenger();

            std::unique_lock<std::mutex> Lock(CountMutex);
            if(Renderers-- == 0) {
                vk::destroyInstance();
            }
        };

        DestroyPermanentResources();
    }

    void Renderer::draw()
    {

    }

    bool Renderer::active() const
    {
        return true;
    }
}