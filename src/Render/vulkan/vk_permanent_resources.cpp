module;
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <unordered_map>
#include "vk_common.hpp"
module Lunar:Renderer;
import :Debug;
import :Window;

namespace DebugMessenger {
    VkDebugUtilsMessengerEXT setup()
    {
#ifdef VK_USE_DEBUG_LAYERS
        VkDebugUtilsMessengerCreateInfoEXT MessengerCreateInfo;
        MessengerCreateInfo = Lunar::vk::debugMessengerInfo();

        auto Function =
            (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                Lunar::vk::getInstance(),
                "vkCreateDebugUtilsMessengerEXT"
            );

        if (Function == NULL) {
            return NULL;
        }

        VkDebugUtilsMessengerEXT Messenger = NULL;
        VkResult Result;

        Result = Function(Lunar::vk::getInstance(), &MessengerCreateInfo, nullptr, &Messenger);
        if (Result != VK_SUCCESS) {
            Lunar::Warn("<Vulkan> Failed to set up a debug messenger.");
        } else Lunar::Print("<Vulkan> Debug messenger was set up.");

        return Messenger;
#endif
    }

    void destroy(VkDebugUtilsMessengerEXT& Messenger)
    {
#ifdef VK_USE_DEBUG_LAYERS
        if (Messenger == NULL) {
            return;
        }
        
        auto Function =
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                Lunar::vk::getInstance(),
                "vkDestroyDebugUtilsMessengerEXT"
            );

        if (Function == NULL) {
            return;
        }

        Function(Lunar::vk::getInstance(), Messenger, nullptr);
        Lunar::Print("<Vulkan> Debug messenger destroyed.");
#endif
    }
}

namespace Surface {
    VkSurfaceKHR create(GLFWwindow* Window)
    {
        VkSurfaceKHR Surface = NULL;
        VkResult Result;
        Result = glfwCreateWindowSurface(
            Lunar::vk::getInstance(),
            Window,
            nullptr,
            &Surface
        );
        if (Result != VK_SUCCESS) {
            Lunar::Error("<Vulkan> Failed to create a surface.");
            Lunar::Exit();
        } else Lunar::Print("<Vulkan> Surface created.");

        return Surface;
    }

    void destroy(VkSurfaceKHR& Surface)
    {
        if (Surface == NULL) {
            return;
        }

        vkDestroySurfaceKHR(Lunar::vk::getInstance(), Surface, nullptr);
        Lunar::Print("<Vulkan> Surface destroyed.");
    }
}

namespace GPU {
    VkPhysicalDevice get()
    {
        Lunar::u32 DeviceCount;
        VkResult Result;
        Result = vkEnumeratePhysicalDevices(Lunar::vk::getInstance(), &DeviceCount.value(), nullptr);
        if (Result != VK_SUCCESS) {
            Lunar::Error("<Vulkan> Failed to enumerate graphics cards.");
            Lunar::Exit();
        }

        std::vector<VkPhysicalDevice> Devices(DeviceCount.value());
        vkEnumeratePhysicalDevices(Lunar::vk::getInstance(), &DeviceCount.value(), Devices.data());

        return NULL;
    }
}

namespace Lunar {
    void Renderer::CreatePermanentResources()
    {
        if (!glfwVulkanSupported()) {
            Lunar::Error("<Vulkan> The machine does not support Vulkan.");
            return;
        }

        m_DebugMsg = DebugMessenger::setup();
        m_Surface = Surface::create(m_Window->m_Handle);
    }

    void Renderer::DestroyPermanentResources()
    {
        Surface::destroy(m_Surface);
        DebugMessenger::destroy(m_DebugMsg);
    }
}
