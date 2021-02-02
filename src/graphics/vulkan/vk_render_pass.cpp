#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>

using namespace std;

namespace Aery {
    bool VkRenderer::CreateRenderPass() {
        vk::AttachmentDescription ColorAttachment = {
            .format = m_Swapchain.format,
            .samples = vk::SampleCountFlagBits::e1,
            .loadOp = vk::AttachmentLoadOp::eClear,
            .storeOp = vk::AttachmentStoreOp::eStore,
            .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
            .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
            .initialLayout = vk::ImageLayout::eUndefined,
            .finalLayout = vk::ImageLayout::ePresentSrcKHR
        };

        vk::AttachmentReference Reference = {
            .attachment = 0,
            .layout = vk::ImageLayout::eColorAttachmentOptimal
        };

        vk::SubpassDescription Subpass = {
            .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
            .colorAttachmentCount = 1,
            .pColorAttachments = &Reference
        };

        vk::SubpassDependency SubpassDependency = {
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,
            .srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
            .dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
            .srcAccessMask = {},
            .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite,
        };

        vk::RenderPassCreateInfo PassInfo = {
            .attachmentCount = 1,
            .pAttachments = &ColorAttachment,
            .subpassCount = 1,
            .pSubpasses = &Subpass,
            .dependencyCount = 1,
            .pDependencies = &SubpassDependency
        };

        vk::Result Result = m_Device.createRenderPass(&PassInfo, nullptr, &m_RenderPass);
        if (Result != vk::Result::eSuccess) {
            Aery::error(fmt::format("<VkRenderer::CreateRenderPass> ID {} failed to create a render pass.", m_ID));
            return false;
        }

        Aery::log(fmt::format("<VkRenderer::CreateRenderPass> ID {} created a render pass.", m_ID), fmt::color::light_green);
        return true;
    }

    void VkRenderer::DestroyRenderPass() {
        m_Device.destroyRenderPass(m_RenderPass);
        Aery::log(fmt::format("<VkRenderer::DestroyRenderPass> ID {} destroyed a render pass.", m_ID));
    }
}