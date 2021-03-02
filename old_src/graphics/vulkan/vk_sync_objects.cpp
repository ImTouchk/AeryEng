#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>
#include <vector>

namespace Aery { namespace Graphics {
    bool VkRenderer::CreateSyncObjects() {
        m_ImageAvailable.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinished.resize(MAX_FRAMES_IN_FLIGHT);
        m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        m_ImagesInFlight.resize(m_Swapchain.images.size(), {});
        vk::SemaphoreCreateInfo SemaphoreInfo = {};
        vk::FenceCreateInfo FenceInfo = {
            .flags = vk::FenceCreateFlagBits::eSignaled
        };
        for (mut_u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            m_ImageAvailable[i] = m_Device.createSemaphore(SemaphoreInfo);
            m_RenderFinished[i] = m_Device.createSemaphore(SemaphoreInfo);
            m_InFlightFences[i] = m_Device.createFence(FenceInfo);
        }
        Aery::log(debug_format("<VkRenderer::CreateSyncObjects> ID {} created all objects.", m_ID), fmt::color::light_green);
        return true;
    }

    void VkRenderer::DestroySyncObjects() {
        for (mut_u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            m_Device.destroySemaphore(m_ImageAvailable[i]);
            m_Device.destroySemaphore(m_RenderFinished[i]);
            m_Device.destroyFence(m_InFlightFences[i]);
            //m_Device.destroyFence(m_ImagesInFlight[i]);
        }
        m_ImageAvailable.clear(); m_RenderFinished.clear();
        m_InFlightFences.clear(); m_ImagesInFlight.clear();
        Aery::log(debug_format("<VkRenderer::DestroySyncObjects> ID {} destroyed all objects.", m_ID));
    }
}
}