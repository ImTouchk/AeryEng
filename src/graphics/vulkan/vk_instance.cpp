#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>
#include <array>

using namespace std;

namespace Aery { namespace Graphics {
    bool VkRenderer::CreateInstance() {
        vector<const char*> Extensions = GetRequiredExtensions(m_States.useLayers);
        if (strcmp(Extensions[0], "NOT_AVAILABLE") == 0) {
            return false;
        }

        vk::ApplicationInfo AppInfo = {
            .pApplicationName = "Untitled Game",
            .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
            .pEngineName = "Aery",
            .engineVersion = VK_MAKE_VERSION(0, 1, 0),
            .apiVersion = VK_API_VERSION_1_0
        };

        vk::InstanceCreateInfo InstInfo = {
            .pApplicationInfo = &AppInfo,
            .enabledLayerCount = 0,
            .enabledExtensionCount = static_cast<u32>(Extensions.size()),
            .ppEnabledExtensionNames = Extensions.data(),

        };

        if (m_States.useLayers && CheckLayerSupport(m_Layers)) {
            vk::DebugUtilsMessengerCreateInfoEXT DebugInfo = EmptyDMCInfo();
            InstInfo.enabledLayerCount = static_cast<u32>(m_Layers.size());
            InstInfo.ppEnabledLayerNames = m_Layers.data();
            InstInfo.pNext = &DebugInfo;
        }

        vk::Result Result = vk::createInstance(&InstInfo, nullptr, &m_Instance);
        if (Result != vk::Result::eSuccess) {
            Aery::error(fmt::format("<VkRenderer::CreateInstance> ID {} failed to create an instance.", m_ID));
        }

        Aery::log(fmt::format("<VkRenderer::CreateInstance> ID {} created an instance.", m_ID), fmt::color::light_green);
        return true;
    }

    void VkRenderer::DestroyInstance() {
        m_Instance.destroy();
        Aery::log(fmt::format("<VkRenderer::DestroyInstance> ID {} destroyed an instance.", m_ID));
    }
}
}