module;
#include <vulkan/vulkan.h>
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#include "vk_common.hpp"
module Lunar:Renderer;

namespace Lunar {
    void Renderer::CreateAllocator()
    {
        VmaAllocatorCreateInfo AllocatorInfo = {
            .physicalDevice = m_PhysDevice,
            .device = m_Device,
            .instance = vk::getInstance(),
            .vulkanApiVersion = VK_API_VERSION_1_0,
        };

        VkResult Result;
        Result = vmaCreateAllocator(&AllocatorInfo, &m_Allocator);
        if (Result != VK_SUCCESS) {
            Lunar::Error("<Renderer> Failed to create an allocator.");
            Lunar::Exit();
        }

        Lunar::Print("<Renderer> Allocator created.");
    }

    void Renderer::DestroyAllocator()
    {
        if (m_Allocator == NULL) {
            return;
        }

        vmaDestroyAllocator(m_Allocator);
        Lunar::Print("<Renderer> Allocator destroyed.");
    }
}