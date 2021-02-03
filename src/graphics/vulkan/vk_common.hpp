#pragma once

#include "utils/types.hpp"
#include <vulkan/vulkan.hpp>
#include <optional>
#include <vector>

namespace Aery {
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
    bool CheckLayerSupport(std::vector<const char*>&);
    std::array<const char*, 2> GetInstanceExtensions();
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
}