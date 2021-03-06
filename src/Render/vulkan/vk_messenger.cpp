module;
#include <vulkan/vulkan.h>
#include "vk_common.hpp"
module Lunar:Renderer;

namespace Lunar {
    void Renderer::SetupMessenger()
    {
#   ifdef VK_USE_DEBUG_LAYERS
        VkDebugUtilsMessengerCreateInfoEXT MessengerCreateInfo;
        MessengerCreateInfo = vk::debugMessengerInfo();

        auto Function =
            (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                vk::getInstance(),
                "vkCreateDebugUtilsMessengerEXT"
            );

        if (Function == NULL) {
            return;
        }

        VkDebugUtilsMessengerEXT Messenger = NULL;
        VkResult Result;

        Result = Function(vk::getInstance(), &MessengerCreateInfo, nullptr, &Messenger);
        if (Result != VK_SUCCESS) {
            Lunar::Warn("<Vulkan> Failed to set up a debug messenger.");
        }
        else Lunar::Print("<Vulkan> Debug messenger was set up.");

        m_DebugMsg = Messenger;
#   endif
    }

    void Renderer::DestroyMessenger()
    {
#   ifdef VK_USE_DEBUG_LAYERS
        if (m_DebugMsg == NULL) {
            return;
        }

        auto Function =
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                vk::getInstance(),
                "vkDestroyDebugUtilsMessengerEXT"
            );

        if (Function == NULL) {
            return;
        }

        Function(vk::getInstance(), m_DebugMsg, nullptr);
        Lunar::Print("<Vulkan> Debug messenger destroyed.");
#   endif
    }
}