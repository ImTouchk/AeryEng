#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vk_shader.hpp"
#include "graphics/vk_object.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>
#include <vector>

using namespace std;

namespace Aery {
    bool VkRenderer::CreateCommandPool() {
        VkQueueFamilyIndices Indices = FindQueueFamilies(m_PhysicalDevice, m_Surface);
        
        vk::CommandPoolCreateInfo CommandPoolInfo = {
            .queueFamilyIndex = Indices.gFamily.value(),
        };

        m_CommandPool = m_Device.createCommandPool(CommandPoolInfo, nullptr);
        Aery::log(fmt::format("<VkRenderer::CreateCommandPool> ID {} created a command pool.", m_ID), fmt::color::light_green);
        return true;
    }

    void VkRenderer::DestroyCommandPool() {
        m_Device.destroyCommandPool(m_CommandPool);
        Aery::log(fmt::format("<VkRenderer::DestroyCommandPool> ID {} destroyed a command pool.", m_ID));
    }

    bool VkRenderer::AllocateCommandBuffers() {
        if (m_CmdBuffersCreated) {
            m_Device.resetCommandPool(m_CommandPool, vk::CommandPoolResetFlagBits::eReleaseResources);
        }

        m_CommandBuffers.resize(m_Swapchain.buffers.size());
        vk::CommandBufferAllocateInfo BufferAllocateInfo = {
            .commandPool = m_CommandPool,
            .level = vk::CommandBufferLevel::ePrimary,
            .commandBufferCount = (u32)m_CommandBuffers.size()
        };

        vk::Result Result = m_Device.allocateCommandBuffers(&BufferAllocateInfo, m_CommandBuffers.data());
        if (Result != vk::Result::eSuccess) {
            Aery::error(fmt::format("<VkRenderer::CreateCmdBuffers> ID {} failed to allocate command buffers.", m_ID));
            return false;
        }
        return true;
    }

    bool VkRenderer::CreateCommandBuffer(int i) {
        vk::CommandBufferBeginInfo BufferBeginInfo = {};
        m_CommandBuffers[i].begin(BufferBeginInfo);

        vk::ClearColorValue ClearColor = {};
        ClearColor.setFloat32({ 0.0f, 0.0f, 0.0f, 1.0f });
        vk::ClearValue ClearValue = {};
        ClearValue.setColor(ClearColor);

        vk::RenderPassBeginInfo PassBeginInfo = {
            .renderPass = m_RenderPass,
            .framebuffer = m_Swapchain.buffers[i],
            .renderArea = {
                .offset = { 0, 0 },
                .extent = m_Swapchain.extent
            },
            .clearValueCount = 1,
            .pClearValues = &ClearValue,
        };

        m_CommandBuffers[i].beginRenderPass(PassBeginInfo, vk::SubpassContents::eInline);
        m_CommandBuffers[i].setViewport(0, 1, &m_Viewport);
        m_CommandBuffers[i].setScissor(0, 1, &m_Scissor);
        for (auto& Object_ : m_Objects) {
            VkObject& Object = Object_.second;
            VkShader& Shader = m_Shaders[Object.shader];

            vk::Buffer VertexBuffers[] = { Object.vertex.buffer };
            vk::DeviceSize Offsets[] = { 0 };

            m_CommandBuffers[i].bindVertexBuffers(0, 1, VertexBuffers, Offsets);
            m_CommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, Shader.pipeline);
            m_CommandBuffers[i].draw(static_cast<u32>(Object.vertex.list.size()), 1, 0, 0);
        }
        m_CommandBuffers[i].endRenderPass();
        m_CommandBuffers[i].end();
        return true;
    }
}