#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace Aery { namespace Graphics {
    u32 VkRenderer::FindMemoryType(u32 TypeFilter, vk::MemoryPropertyFlags Properties) {
        vk::PhysicalDeviceMemoryProperties MemoryProperties = {};
        m_PhysicalDevice.getMemoryProperties(&MemoryProperties);

        for (mut_u16 i = 0; i < MemoryProperties.memoryTypeCount; i++) {
            if (TypeFilter & (1 << i) && (MemoryProperties.memoryTypes[i].propertyFlags & Properties) == Properties) {
                return i;
            }
        }
        return -1;
    }
}
}