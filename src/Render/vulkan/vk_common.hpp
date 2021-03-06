#pragma once

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

import Lunar;

namespace Lunar::vk {
    VkInstance& getInstance();
    void destroyInstance();

    VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo();

    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT Severity,
        VkDebugUtilsMessageTypeFlagsEXT Type,
        const VkDebugUtilsMessengerCallbackDataEXT* CallbackData,
        void* UserData
    );

    struct queueFamilyIndices {
        std::optional<Lunar::u32> gFamily;
        std::optional<Lunar::u32> pFamily;
        inline bool isComplete() {
            return gFamily.has_value() && pFamily.has_value();
        }
    };

    struct swapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkPresentModeKHR> presentModes;
        std::vector<VkSurfaceFormatKHR> formats;
    };

    queueFamilyIndices& findQueueFamilies(
        VkPhysicalDevice& Device,
        VkSurfaceKHR& Surface
    );

    swapchainSupportDetails& querySwapSupport(
        VkPhysicalDevice& Device,
        VkSurfaceKHR& Surface
    );

}
