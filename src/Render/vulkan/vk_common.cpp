#include "vk_common.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vector>
#include <array>

import Lunar;

namespace {
    VkInstance GlobalInstance = NULL;

    std::vector<const char*> GetRequiredExtensions()
    {
        Lunar::u32 ExtCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&ExtCount.value());
        std::vector<const char*> Extensions(glfwExtensions, glfwExtensions + ExtCount.value());
#       ifdef VK_USE_DEBUG_LAYERS
        Extensions.push_back(
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        );
#       endif
        return Extensions;
    }

    bool CheckLayerSupport(std::array<const char*, 1>& Layers)
    {
        Lunar::u32 LayerCount = 0;
        VkResult Result;

        Result = vkEnumerateInstanceLayerProperties(&LayerCount.value(), nullptr);
        if (Result != VK_SUCCESS) {
            Lunar::Error("<CheckLayerSupport> Could not enumerate layer properties.");
            return false;
        }

        std::vector<VkLayerProperties> Properties(LayerCount.value());
        vkEnumerateInstanceLayerProperties(&LayerCount.value(), Properties.data());

        for (Lunar::u32 Current : Lunar::range(Layers.size())) {
            bool LayerFound = false;
            for (Lunar::u32 Available : Lunar::range(Layers.size())) {
                if (strcmp(Layers[Current], Properties[Available].layerName) == 0) {
                    LayerFound = true;
                    break;
                }
            }

            if (!LayerFound) {
                return false;
            }
        }

        return true;
    }
}

namespace Lunar::vk {
    VkInstance& getInstance()
    {
        if (GlobalInstance == NULL) {
            std::vector<const char*> Ext = GetRequiredExtensions();
            std::array<const char*, 1> DebugLayers = {
                "VK_LAYER_KHRONOS_validation"
            };

            VkApplicationInfo ApplicationInfo = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = "Untitled",
                .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
                .pEngineName = "Lunar",
                .engineVersion = VK_MAKE_VERSION(0, 0, 1),
                .apiVersion = VK_API_VERSION_1_0,
            };

            VkInstanceCreateInfo InstanceCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pApplicationInfo = &ApplicationInfo,
                .enabledLayerCount = 0,
                .enabledExtensionCount = static_cast<uint32_t>(Ext.size()),
                .ppEnabledExtensionNames = Ext.data(),
            };

#           ifdef VK_USE_DEBUG_LAYERS
            if (CheckLayerSupport(DebugLayers)) {
                VkDebugUtilsMessengerCreateInfoEXT MessengerCreateInfo = debugMessengerInfo();
                InstanceCreateInfo.enabledLayerCount = static_cast<u32>(DebugLayers.size());
                InstanceCreateInfo.ppEnabledLayerNames = DebugLayers.data();
                InstanceCreateInfo.pNext = &MessengerCreateInfo;
            }
#           endif

            VkResult Result;
            Result = vkCreateInstance(&InstanceCreateInfo, nullptr, &GlobalInstance);
            if (Result != VK_SUCCESS) {
                Lunar::Error("<Vulkan> Failed to create an instance.");
            }

            Lunar::Print("<Vulkan> Instance created.");
        }
        return GlobalInstance;
    }

    void destroyInstance()
    {
        if (GlobalInstance == NULL) {
            return;
        }

        vkDestroyInstance(GlobalInstance, nullptr);
        Lunar::Print("<Vulkan> Instance destroyed.");
    }
}

namespace Lunar::vk {
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT Severity, 
        VkDebugUtilsMessageTypeFlagsEXT Type, 
        const VkDebugUtilsMessengerCallbackDataEXT* CallbackData,
        void* UserData
    )
    {
        if(Severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            Lunar::Warn("<VULKAN LAYER> {}", CallbackData->pMessage);

        if (Severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            Lunar::Error("<VULKAN LAYER> {}", CallbackData->pMessage);

        return VK_FALSE;
    }

    VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo()
    {
        VkDebugUtilsMessengerCreateInfoEXT CreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,

            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT   |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,

            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
            .pfnUserCallback = Lunar::vk::debugCallback,
            .pUserData = NULL
        };
        return CreateInfo;
    }

    queueFamilyIndices& findQueueFamilies(VkPhysicalDevice& Device, VkSurfaceKHR& Surface)
    {
        /*  
            The result is queried each time the swapchain
            is recreated, so caching the result is ok
        */

        static struct {
            VkPhysicalDevice gpu = NULL;
            queueFamilyIndices indices;
        } Cache;

        if (Cache.gpu == Device) {
            return Cache.indices;
        }

        Cache.gpu = Device;

        Lunar::u32 FamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(Device, &FamilyCount.value(), nullptr);

        VkQueueFamilyProperties* Families = new VkQueueFamilyProperties[FamilyCount.value()];
        vkGetPhysicalDeviceQueueFamilyProperties(Device, &FamilyCount.value(), Families);
        

        for (Lunar::u32 i : Lunar::range(FamilyCount)) {
            VkBool32 PresentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(Device, i, Surface, &PresentSupport);
            if (Families[i.value()].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                Cache.indices.gFamily = i;
            }
            if (PresentSupport) {
                Cache.indices.pFamily = i;
            }
        }

        delete[] Families;
        return Cache.indices;
    }

    swapchainSupportDetails& querySwapSupport(VkPhysicalDevice& Device, VkSurfaceKHR& Surface)
    {
        /* Same thing happens here */

        static VkPhysicalDevice CachedGPU = NULL;
        static swapchainSupportDetails Cache = {};

        if (CachedGPU == Device) {
            return Cache;
        }

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, Surface, &Cache.capabilities);

        Lunar::u32 FormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(Device, Surface, &FormatCount.value(), nullptr);
        if (FormatCount != 0) {
            Cache.formats.resize(FormatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                Device, 
                Surface, 
                &FormatCount.value(), 
                Cache.formats.data()
            );
        }

        Lunar::u32 PresentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(Device, Surface, &PresentModeCount.value(), nullptr);
        if (PresentModeCount != 0) {
            Cache.presentModes.resize(PresentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                Device, 
                Surface, 
                &PresentModeCount.value(), 
                Cache.presentModes.data()
            );
        }

        return Cache;
    }
}
