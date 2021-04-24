#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <cstring>

#include "types.hpp"
#include "debug.hpp"
#include "graphics/renderer.hpp"

using LayersArray = std::array<const char*, 1>;

namespace {
    std::vector<const char*> getRequiredExtensions()
    {
        const char** glfwExtensions;
        usize        extCount = 0;
        glfwExtensions        = glfwGetRequiredInstanceExtensions(&extCount);
        std::vector<const char*> extensions(
            glfwExtensions,
            glfwExtensions + extCount
        );

#       ifndef NDEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#       endif

        return extensions;
    }

    bool checkLayerSupport(LayersArray& layers)
    {
        VkResult result;
        usize    layerCount = 0;

        result = vkEnumerateInstanceLayerProperties(
            &layerCount,
            NULL
        );

        if(result != VK_SUCCESS) {
            return false;
        }

        std::vector<VkLayerProperties> properties(layerCount);
        vkEnumerateInstanceLayerProperties(
            &layerCount,
            properties.data()
        );

        for(const auto& needed : layers) {
            bool layerFound = false;
            for(const auto& available : properties) {
                if(strcmp(needed, available.layerName) == 0) {
                    layerFound = true;
                }
            }
            if(!layerFound)
                return false;
        }

        return true;
    }
}

namespace {
    VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT* pData,
        void* pUserData
    )
    {
        if(severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            Lunar::warn("Vulkan layer: {}.", pData->pMessage);
        else if(severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            Lunar::error("Vulkan layer: {}.", pData->pMessage);

        return VK_FALSE;
    }

    VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo()
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {
            .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext           = nullptr,
            .flags           = 0,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
            .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
            .pfnUserCallback = vulkanDebugCallback,
            .pUserData       = nullptr,
        };
        return createInfo;
    }
}

namespace {
    isize      REFERENCE_COUNT = 0;
    VkInstance GLOBAL_INSTANCE = nullptr;
    
    VkInstance& getVulkanInstance()
    {
        if(GLOBAL_INSTANCE == nullptr) {
            std::vector<const char*> extensions = getRequiredExtensions();

            VkApplicationInfo appInfo = {
                .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext              = nullptr,
                .pApplicationName   = "LunarEngineGame",
                .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
                .pEngineName        = "Lunar",
                .engineVersion      = VK_MAKE_VERSION(0, 0, 1),
                .apiVersion         = VK_API_VERSION_1_0
            };

            VkInstanceCreateInfo instanceCreateInfo = {
                .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pNext                   = nullptr,
                .flags                   = 0,
                .pApplicationInfo        = &appInfo,
                .enabledLayerCount       = 0,
                .ppEnabledLayerNames     = 0,
                .enabledExtensionCount   = static_cast<uint32_t>(extensions.size()),
                .ppEnabledExtensionNames = extensions.data()
            };

#           ifndef NDEBUG
            LayersArray layers = { "VK_LAYER_KHRONOS_validation" };
            if(checkLayerSupport(layers)) {
                VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = debugMessengerInfo();
                instanceCreateInfo.enabledLayerCount   = static_cast<uint32_t>(layers.size()),
                instanceCreateInfo.ppEnabledLayerNames = layers.data(),
                instanceCreateInfo.pNext               = &messengerCreateInfo;
            }
#           endif

            VkResult result;
            result = vkCreateInstance(&instanceCreateInfo, nullptr, &GLOBAL_INSTANCE);
            if(result != VK_SUCCESS) {
                Lunar::error("Renderer: Failed to create a vulkan instance.");
                exit(-1);
            }

            Lunar::log("Renderer: Vulkan instance created.");
        }

        return GLOBAL_INSTANCE;
    }

    void destroyVulkanInstance()
    {
        vkDestroyInstance(GLOBAL_INSTANCE, nullptr);
        Lunar::log("Renderer: Vulkan instance destroyed.");
    }

    void incrementCounter()
    {
        REFERENCE_COUNT++;
        if(GLOBAL_INSTANCE == nullptr)
            getVulkanInstance(); // first call to getVulkanInstance initializes it
    }

    void decrementCounter()
    {
        REFERENCE_COUNT--;
        if(REFERENCE_COUNT == 0)
            destroyVulkanInstance();
    }
}

namespace Lunar {
    void renderer::createBoilerplate()
    {
        incrementCounter();
        setupDbgMessenger();
    }

    void renderer::destroyBoilerplate()
    {
        destroyDbgMessenger();
        decrementCounter();
    }

    void renderer::setupDbgMessenger()
    {
#       ifndef NDEBUG
        VkDebugUtilsMessengerCreateInfoEXT createInfo = debugMessengerInfo();
        auto function = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            getVulkanInstance(), "vkCreateDebugUtilsMessengerEXT"
        );

        VkResult result;
        if(function == nullptr) {
            result = VK_ERROR_EXTENSION_NOT_PRESENT;
        } else {
            result = function(
                getVulkanInstance(), 
                &createInfo, 
                nullptr, 
                &m_DebugMessenger
            );
        }

        if(result != VK_SUCCESS) {
            Lunar::warn("Renderer: Could not initialize vulkan debug layers.");
            return;
        }

        Lunar::log("Renderer: Vulkan debug layers initialized.");
#       endif
    }

    void renderer::destroyDbgMessenger()
    {
#       ifndef NDEBUG
        auto function = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            getVulkanInstance(), "vkDestroyDebugUtilsMessengerEXT"
        );
        if(function != nullptr && m_DebugMessenger != nullptr) {
            function(
                getVulkanInstance(),
                m_DebugMessenger,
                nullptr
            );
            Lunar::log("Renderer: Vulkan debug layers destroyed.");
        }
#       endif
    }
}
