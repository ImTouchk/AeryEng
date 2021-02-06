#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>

using namespace std;

namespace Aery { namespace Graphics {
    bool VkRenderer::CreateFramebuffers() {
        m_Swapchain.buffers.resize(m_Swapchain.views.size());
        for (mut_u32 i = 0; i < m_Swapchain.views.size(); i++) {
            vk::ImageView Attachments[] = {
                m_Swapchain.views[i]
            };

            vk::FramebufferCreateInfo FramebufferInfo = {
                .renderPass = m_RenderPass,
                .attachmentCount = 1,
                .pAttachments = Attachments,
                .width = m_Swapchain.extent.width,
                .height = m_Swapchain.extent.height,
                .layers = 1
            };

            vk::Result Result = m_Device.createFramebuffer(&FramebufferInfo, nullptr, &m_Swapchain.buffers[i]);
            if (Result != vk::Result::eSuccess) {
                Aery::error(fmt::format("<VkRenderer::CreateFramebuffers> ID {} failed to create a framebuffer.", m_ID));
                return false;
            }
        }

        Aery::log(fmt::format("<VkRenderer::CreateFramebuffers> ID {} created {} framebuffers.", m_ID, m_Swapchain.buffers.size()), fmt::color::light_green);
        return true;
    }

    void VkRenderer::DestroyFramebuffers() {
        for (mut_u32 i = 0; i < m_Swapchain.buffers.size(); i++) {
            m_Device.destroyFramebuffer(m_Swapchain.buffers[i]);
        }
        Aery::log(fmt::format("<VkRenderer::DestroyFramebuffers> ID {} destroyed {} framebuffers.", m_ID, m_Swapchain.buffers.size()));
        m_Swapchain.buffers.clear();
    }
}
}