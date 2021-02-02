#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan.h>
#include <fmt/core.h>

namespace Aery {
    bool VkRenderer::SetupDM() {
        if (!m_UseLayers) {
            Aery::log(fmt::format("<VkRenderer::SetupDM> ID {} is not using debug layers.", m_ID));
            m_LayersUsed = false;
            return false;
        }

        VkDebugUtilsMessengerCreateInfoEXT CreateInfo = static_cast<VkDebugUtilsMessengerCreateInfoEXT>(EmptyDMCInfo());
        auto Function = (PFN_vkCreateDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(static_cast<VkInstance>(m_Instance), "vkCreateDebugUtilsMessengerEXT");

        VkDebugUtilsMessengerEXT DMessenger = {};
        VkResult Result;
        if (Function == nullptr) {
            Result = VK_ERROR_EXTENSION_NOT_PRESENT;
        } else {
            Result = Function(static_cast<VkInstance>(m_Instance), &CreateInfo, nullptr, &DMessenger);
        }
        if (Result != VK_SUCCESS) {
            Aery::warn(fmt::format("<VkRenderer::SetupDM> ID {} failed to set up a debug messenger.", m_ID));
            m_LayersUsed = false;
            return false;
        }

        m_DebugMessenger = static_cast<vk::DebugUtilsMessengerEXT>(DMessenger);
        m_LayersUsed = true;
        Aery::log(fmt::format("<VkRenderer::SetupDM> ID {} set up a debug messenger.", m_ID), fmt::color::light_green);
        return true;
    }

    void VkRenderer::DestroyDM() {
        if (!m_LayersUsed) {
            return;
        }
        auto Function = (PFN_vkDestroyDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(static_cast<VkInstance>(m_Instance), "vkDestroyDebugUtilsMessengerEXT");
        if (Function != nullptr) {
            Function(static_cast<VkInstance>(m_Instance), static_cast<VkDebugUtilsMessengerEXT>(m_DebugMessenger), nullptr);
        }
        Aery::log(fmt::format("<VkRenderer::DestroyDM> ID {} destroyed a debug messenger.", m_ID));
    }
}
