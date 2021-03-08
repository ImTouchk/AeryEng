#include <vulkan/vulkan.h>
#include <vector>
#include <set>

#include "Types.h"
#include "Debug.h"
#include "Graphics/VkCommon.h"
#include "Graphics/Renderer.h"

namespace Lunar {
    void Renderer::CreateDevice()
    {
        const vk::queueFamilyIndices Indices = vk::findQueueFamilies(m_GPU, m_Surface);

        std::vector<VkDeviceQueueCreateInfo> QueueInfos;
        std::set<u32> UniqueFamilies = {
            Indices.gFamily.value(),
            Indices.pFamily.value()
        };

        f32 QueuePriority = 1.0f;
        for (u32 i : range(UniqueFamilies.size())) {
            VkDeviceQueueCreateInfo QueueInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = i,
                .queueCount = 1,
                .pQueuePriorities = &QueuePriority
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
        Result = vkCreateDevice(m_GPU, &DeviceInfo, NULL, &Device);
        if (Result != VK_SUCCESS) {
            Lunar::Error("Renderer> Failed to create a logical device.");
            Lunar::Exit();
        }

        vkGetDeviceQueue(Device, Indices.gFamily.value(), 0, &m_GraphicsQ);
        vkGetDeviceQueue(Device, Indices.pFamily.value(), 0, &m_PresentQ);

        m_Device = Device;
        Lunar::Print("Renderer> Logical device created.");
    }

    void Renderer::DestroyDevice()
    {
        if (m_Device == NULL) {
            return;
        }
        vkDestroyDevice(m_Device, NULL);
        Lunar::Print("Renderer> Logical device destroyed.");
    }
}
