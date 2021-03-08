#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <array>

#include "Debug.h"
#include "Types.h"
#include "Graphics/VkCommon.h"

#define VK_USE_DEBUG_LAYERS

namespace {
    VkInstance GlobalInstance = NULL;

    std::vector<const char*> GetRequiredExtensions()
    {
        u32 ExtCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&ExtCount);
        std::vector<const char*> Extensions(glfwExtensions, glfwExtensions + ExtCount);
#       ifdef VK_USE_DEBUG_LAYERS
        Extensions.push_back(
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        );
#       endif
        return Extensions;
    }

    bool CheckLayerSupport(std::array<const char*, 1>& Layers)
    {
        u32 LayerCount = 0;
        VkResult Result;

        Result = vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);
        if (Result != VK_SUCCESS) {
            Lunar::Error("Renderer - Could not enumerate layer properties.");
            return false;
        }

        std::vector<VkLayerProperties> Properties(LayerCount);
        vkEnumerateInstanceLayerProperties(&LayerCount, Properties.data());

        for (u32 Current : range( u32( Layers.size() ))) {
            bool LayerFound = false;
            for (u32 Available : range( u32(Layers.size()))) {
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
                .enabledExtensionCount = static_cast<u32>(Ext.size()),
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
                Error("Renderer> Failed to create an instance.");
            }

            Print("Renderer> Instance created.");
        }
        return GlobalInstance;
    }

    void destroyInstance()
    {
        if (GlobalInstance == NULL) {
            return;
        }

        vkDestroyInstance(GlobalInstance, nullptr);
        Print("Renderer> Instance destroyed.");
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
            Warn("VULKAN> {}", CallbackData->pMessage);

        if (Severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            Error("VULKAN> {}", CallbackData->pMessage);

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
            .pfnUserCallback = vk::debugCallback,
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
            Lunar::Print("Vulkan> Using cached queue families result.");
            return Cache.indices;
        }

        Cache.gpu = Device;

        u32 FamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(Device, &FamilyCount, nullptr);

        VkQueueFamilyProperties* Families = new VkQueueFamilyProperties[FamilyCount];
        vkGetPhysicalDeviceQueueFamilyProperties(Device, &FamilyCount, Families);
        
        for (u32 i : range(FamilyCount)) {
            VkBool32 PresentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(Device, i, Surface, &PresentSupport);
            if (PresentSupport) { Cache.indices.pFamily = i; }
            if (Families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                Cache.indices.gFamily = i;
            }
            if (Cache.indices.isComplete()) {
                break;
            }
        }

        delete[] Families;
        return Cache.indices;
    }

    swapchainSupportDetails& querySwapSupport(VkPhysicalDevice& Device, VkSurfaceKHR& Surface)
    {
        /* Same thing happens here */
        static struct {
            VkPhysicalDevice gpu = NULL;
            swapchainSupportDetails details;
        } Cache;

        if (Cache.gpu == Device) {
            Lunar::Print("Vulkan> Using cached swapchain support result.");
            return Cache.details;
        }

        Cache.gpu = Device;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, Surface, &Cache.details.capabilities);

        u32 FormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(Device, Surface, &FormatCount, nullptr);
        if (FormatCount != 0) {
            Cache.details.formats.resize(FormatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                Device, 
                Surface, 
                &FormatCount, 
                Cache.details.formats.data()
            );
        }

        u32 PresentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(Device, Surface, &PresentModeCount, nullptr);
        if (PresentModeCount != 0) {
            Cache.details.presentModes.resize(PresentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                Device, 
                Surface, 
                &PresentModeCount, 
                Cache.details.presentModes.data()
            );
        }

        return Cache.details;
    }
}