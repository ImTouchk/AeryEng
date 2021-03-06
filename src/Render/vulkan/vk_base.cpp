module;
#include <vulkan/vulkan.h>
#include "vk_common.hpp"
module Lunar:Renderer;

namespace Lunar {
    bool Renderer::start()
    {
        vk::getInstance();
        return true;
    }

    void Renderer::stop()
    {
        vk::destroyInstance();
    }

    void Renderer::draw()
    {

    }
}