module;
#include "vk_common.hpp"
#include <vulkan/vulkan.h>
#include <fmt/color.h>
#include <mutex>
#include <array>
module Lunar:Renderer;
import :Numbers;
import :Debug;

namespace {
    Lunar::u32 Renderers = 0;
    std::mutex CountMutex;
}

namespace Lunar {
    bool Renderer::start(Lunar::Window& Window)
    {
        CountMutex.lock();
        Renderers++;
        m_ID = Renderers;
        CountMutex.unlock();

        m_Window = &Window;

        Lunar::PrintColor(fmt::color::light_green);
        Lunar::Print("------- CREATING RENDERER {} -------", m_ID.value());
        Lunar::PrintColor(fmt::color::white);
        CreatePermanentResources();
        return true;
    }

    void Renderer::stop()
    {
        Lunar::PrintColor(fmt::color::light_sky_blue);
        Lunar::Print("------- DESTROYING RENDERER {} -------", m_ID.value());
        Lunar::PrintColor(fmt::color::white);

        DestroyPermanentResources();

        CountMutex.lock();
        Renderers--;
        if (Renderers == 0) {
            vk::destroyInstance();
        }
        CountMutex.unlock();
    }

    void Renderer::draw()
    {

    }

    void Renderer::CreatePermanentResources()
    {
        if (!glfwVulkanSupported()) {
            Lunar::Error("<Renderer> The machine does not support Vulkan.");
            return;
        }

        SetupMessenger();
        CreateSurface();
        PickGPU();
        CreateDevice();
        CreateAllocator();
    }

    void Renderer::DestroyPermanentResources()
    {
        DestroyAllocator();
        DestroyDevice();
        DestroySurface();
        DestroyMessenger();
    }
}
