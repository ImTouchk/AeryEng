#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vk_shader.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>
#include <vector>

using namespace std;

namespace Aery {
    bool VkRenderer::CreateSemaphores() {
        vk::SemaphoreCreateInfo SemaphoreInfo = {};
        m_ImageAvailable = m_Device.createSemaphore(SemaphoreInfo);
        m_RenderFinished = m_Device.createSemaphore(SemaphoreInfo);
        return true;
    }

    void VkRenderer::DestroySemaphores() {
        m_Device.destroySemaphore(m_ImageAvailable);
        m_Device.destroySemaphore(m_RenderFinished);
    }
}