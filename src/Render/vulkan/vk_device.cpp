module;
#include <set>
#include <vector>
#include <vulkan/vulkan.h>
#include "vk_common.hpp"
module Lunar:Renderer;
import :Numbers;

namespace Lunar {
    void Renderer::CreateDevice()
    {
        const vk::queueFamilyIndices Indices = vk::findQueueFamilies(m_PhysDevice, m_Surface);

        std::vector<VkDeviceQueueCreateInfo> QueueInfos;
        std::set<Lunar::u32> UniqueFamilies = {
            Indices.gFamily.value(),
            Indices.pFamily.value()
        };

        Lunar::f32 QueuePriority = 1.0f;
        for (Lunar::u32 i : Lunar::range(UniqueFamilies.size())) {
            VkDeviceQueueCreateInfo QueueInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = i,
                .queueCount = 1,
                .pQueuePriorities = &QueuePriority.value()
            };
            QueueInfos.push_back(QueueInfo);
        }

        VkPhysicalDeviceFeatures Features = {};
        VkDeviceCreateInfo DeviceInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = NULL,
            .queueCreateInfoCount = static_cast<uint32_t>(QueueInfos.size()),
            .pQueueCreateInfos = QueueInfos.data(),
            .enabledLayerCount = 0,
            .enabledExtensionCount = static_cast<uint32_t>(m_Extensions.size()),
            .ppEnabledExtensionNames = m_Extensions.data(),
            .pEnabledFeatures = &Features
        };

#   ifdef VK_USE_DEBUG_LAYERS
        DeviceInfo.enabledLayerCount = static_cast<uint32_t>(m_Layers.size());
        DeviceInfo.ppEnabledLayerNames = m_Layers.data();
#   endif

        VkDevice Device;
        VkResult Result;
        Result = vkCreateDevice(m_PhysDevice, &DeviceInfo, nullptr, &Device);
        if (Result != VK_SUCCESS) {
            Lunar::Error("<Renderer> Failed to create a logical device.");
            Lunar::Exit();
        }

        vkGetDeviceQueue(Device, Indices.gFamily.value(), 0, &m_GraphicsQ);
        vkGetDeviceQueue(Device, Indices.pFamily.value(), 0, &m_PresentQ);

        m_Device = Device;
        Lunar::Print("<Renderer> Logical device created.");
    }

    void Renderer::DestroyDevice()
    {
        if (m_Device == NULL) {
            return;
        }
        vkDestroyDevice(m_Device, nullptr);
        Lunar::Print("<Renderer> Logical device destroyed.");
    }
}

