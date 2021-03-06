#pragma once

#include "utils/types.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <optional>
#include <vector>

namespace Aery { namespace Graphics {
    struct VkQueueFamilyIndices {
        std::optional<mut_u32> gFamily;
        std::optional<mut_u32> pFamily;
        inline bool isComplete() {
            return  gFamily.has_value() &&
                pFamily.has_value();
        }
    };

    struct VkSwapchainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::PresentModeKHR> presentModes;
        std::vector<vk::SurfaceFormatKHR> formats;
    };

    class Window;
    bool CreateVulkanSurface(Window&);
    void DestroyVulkanSurface(Window&);
    bool CheckLayerSupport(std::array<const char*, 1>&);
    std::vector<const char*> GetRequiredExtensions(bool);
    vk::DebugUtilsMessengerCreateInfoEXT EmptyDMCInfo();

    VkQueueFamilyIndices& FindQueueFamilies(
        vk::PhysicalDevice&,
        vk::SurfaceKHR&
    );

    VkSwapchainSupportDetails& QuerySwapSupport(
        vk::PhysicalDevice&,
        vk::SurfaceKHR& 
    );

    bool CreateGPUBuffer(
        vk::BufferUsageFlags,
        void*,
        vk::DeviceSize,
        vk::Device&,
        vk::CommandPool&,
        VmaAllocator&,
        VmaAllocation&,
        vk::Queue&,
        vk::Buffer&
    );
}
}