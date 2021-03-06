module;
#include "vk_common.hpp"
#include <vulkan/vulkan.h>
#include <mutex>
module Lunar:Renderer;
import :Numbers;

namespace {
    Lunar::u32 Renderers = 0;
    std::mutex CountMutex;
}

namespace Lunar {
    bool Renderer::start(Lunar::Window& Window)
    {
        CountMutex.lock();
        Renderers++;
        CountMutex.unlock();

        m_Window = &Window;
        CreatePermanentResources();
        return true;
    }

    void Renderer::stop()
    {
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
}