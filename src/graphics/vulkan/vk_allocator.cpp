#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

using namespace std;

namespace Aery {
    bool VkRenderer::CreateAllocator() {
        VmaAllocatorCreateInfo AllocatorInfo = {
            .physicalDevice = static_cast<VkPhysicalDevice>(m_PhysicalDevice),
            .device = static_cast<VkDevice>(m_Device),
            .instance = static_cast<VkInstance>(m_Instance),
            .vulkanApiVersion = VK_API_VERSION_1_0,
        };
        VkResult Result = vmaCreateAllocator(&AllocatorInfo, &m_Allocator);
        if (Result != VK_SUCCESS) {
            Aery::error(fmt::format("<VkRenderer::CreateAllocator> ID {} failed to create an allocator.", m_ID));
            return false;
        }

        Aery::log(fmt::format("<VkRenderer::CreateAllocator> ID {} created an allocator.", m_ID), fmt::color::light_green);
        return true;
    }

    void VkRenderer::DestroyAllocator() {
        vmaDestroyAllocator(m_Allocator);
        Aery::log(fmt::format("<VkRenderer::DestroyAllocator> ID {} destroyed an allocator.", m_ID));
    }
}