#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>
#include <vector>
#include <set>

using namespace std;

namespace Aery { namespace Graphics {
    bool VkRenderer::CreateLogicalDevice() {
        const VkQueueFamilyIndices Indices = FindQueueFamilies(m_PhysicalDevice, m_Surface);
        vector<vk::DeviceQueueCreateInfo> QueueInfos = {};
        const set<mut_u32> UniqueQFamilies = {
            Indices.gFamily.value(),
            Indices.pFamily.value()
        };

        const f32 QueuePriority = 1.0f;
        for (mut_u32 i = 0; i < UniqueQFamilies.size(); i++) {
            vk::DeviceQueueCreateInfo QueueInfo = {
                .queueFamilyIndex = i,
                .queueCount = 1,
                .pQueuePriorities = &QueuePriority,
            };
            QueueInfos.push_back(QueueInfo);
        }

        vk::PhysicalDeviceFeatures Features = {};
        vk::DeviceCreateInfo DeviceInfo = {
            .pNext = nullptr,
            .queueCreateInfoCount = static_cast<u32>(QueueInfos.size()),
            .pQueueCreateInfos = QueueInfos.data(),
            .enabledLayerCount = 0,
            .enabledExtensionCount = static_cast<u32>(m_Extensions.size()),
            .ppEnabledExtensionNames = m_Extensions.data(),
            .pEnabledFeatures = &Features,
        };

        if (m_States.layersUsed) {
            DeviceInfo.enabledLayerCount = static_cast<u32>(m_Layers.size());
            DeviceInfo.ppEnabledLayerNames = m_Layers.data();
        }

        vk::Result Result = m_PhysicalDevice.createDevice(&DeviceInfo, nullptr, &m_Device);
        if (Result != vk::Result::eSuccess) {
            Aery::error(fmt::format("<VkRenderer::CreateLogicalDevice> ID {} failed to create a logical device.", m_ID));
            return false;
        }

        m_Device.getQueue(Indices.gFamily.value(), 0, &m_GraphicsQ);
        m_Device.getQueue(Indices.pFamily.value(), 0, &m_PresentQ);
        Aery::log(fmt::format("<VkRenderer::CreateLogicalDevice> ID {} created a logical device.", m_ID), fmt::color::light_green);
        return true;
    }

    void VkRenderer::DestroyLogicalDevice() {
        m_Device.destroy();
        Aery::log(fmt::format("<VkRenderer::DestroyLogicalDevice> ID {} destroyed a logical device.", m_ID));
    }
}
}