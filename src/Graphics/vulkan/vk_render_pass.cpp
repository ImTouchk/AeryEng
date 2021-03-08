#include <vulkan/vulkan.h>

#include "Debug.h"
#include "Types.h"
#include "Graphics/Renderer.h"

namespace Lunar {
    void Renderer::CreateRenderPass()
    {
        VkAttachmentDescription ColorAttachment = {
            .format = m_SwapFormat,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        };

        VkAttachmentReference Reference = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        VkSubpassDescription SubpassDesc = {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &Reference,
        };

        VkSubpassDependency SubpassDep = {
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = {},
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        };

        VkRenderPassCreateInfo PassCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = 1,
            .pAttachments = &ColorAttachment,
            .subpassCount = 1,
            .pSubpasses = &SubpassDesc,
            .dependencyCount = 1,
            .pDependencies = &SubpassDep,
        };

        VkResult Result;
        Result = vkCreateRenderPass(m_Device, &PassCreateInfo, NULL, &m_RenderPass);
        if(Result != VK_SUCCESS) {
            Lunar::Error("Renderer> Failed to create a render pass.");
            Lunar::Exit();
        }

        Lunar::Print("Renderer> Render pass created.");
    }

    void Renderer::DestroyRenderPass()
    {
        if(m_Device == NULL || m_RenderPass == NULL) {
            return;
        }

        vkDestroyRenderPass(m_Device, m_RenderPass, NULL);
        Lunar::Print("Renderer> Render pass destroyed.");
    }
}