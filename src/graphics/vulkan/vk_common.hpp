#pragma once

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
    bool CheckLayerSupport(std::vector<const char*>& Layers);
    std::array<const char*, 2> GetInstanceExtensions();
    std::vector<const char*> GetRequiredExtensions(bool UseLayers);
    vk::DebugUtilsMessengerCreateInfoEXT EmptyDMCInfo();

    VkQueueFamilyIndices& FindQueueFamilies(
        vk::PhysicalDevice& GPU,
        vk::SurfaceKHR& Surface
    );

    VkSwapchainSupportDetails& QuerySwapSupport(
        vk::PhysicalDevice& GPU,
        vk::SurfaceKHR& Surface
    );
}