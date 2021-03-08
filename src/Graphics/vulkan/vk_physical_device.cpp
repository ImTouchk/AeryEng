#include <vulkan/vulkan.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <array>
#include <set>

#include "Debug.h"
#include "Types.h"
#include "Graphics/VkCommon.h"
#include "Graphics/Renderer.h"

namespace {
    u32 DeviceScore(VkPhysicalDevice& Device, VkSurfaceKHR& Surface, const std::array<const char*, 1>& Ext)
    {
        u32 Score = 0;
        u32 ExtCount = 0;

        VkResult Result;
        Result = vkEnumerateDeviceExtensionProperties(Device, NULL, &ExtCount, NULL);
        if (Result != VK_SUCCESS) {
            return 0;
        }

        VkExtensionProperties* Extensions = new VkExtensionProperties[ExtCount];
        vkEnumerateDeviceExtensionProperties(Device, NULL, &ExtCount, Extensions);

        std::set<std::string> Required(Ext.begin(), Ext.end());
        for (u32 i : range(ExtCount)) {
            Required.erase(Extensions[i].extensionName);
        }
        delete[] Extensions;

        VkPhysicalDeviceProperties Properties;
        vkGetPhysicalDeviceProperties(Device, &Properties);

        VkPhysicalDeviceMemoryProperties Memory;
        vkGetPhysicalDeviceMemoryProperties(Device, &Memory);

        for (u32 i : range(Memory.memoryHeapCount)) {
            VkMemoryHeap* Heap;
            Heap = &( Memory.memoryHeaps[i] );

            if (Heap->flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                Score = Score + (Heap->size);
            }
        }

        switch (Properties.deviceType) {
        default: {
            Score = Score + 1; 
            break;
        }
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: {
            Score = Score + 100; 
            break;
        }
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: {
            Score = Score + 50; 
            break;
        }
        }

        Lunar::vk::swapchainSupportDetails SwapSupport;
        Lunar::vk::queueFamilyIndices Indices;

        SwapSupport = Lunar::vk::querySwapSupport(Device, Surface);
        Indices = Lunar::vk::findQueueFamilies(Device, Surface);
        bool SwapAdequate = !SwapSupport.formats.empty() && !SwapSupport.presentModes.empty();
        bool DeviceAdequate = Indices.isComplete() && SwapAdequate && Required.size() == 0;
        if (!SwapAdequate) {
            Score = 0;
        }
        return Score;
    }
}

namespace Lunar {
    void Renderer::PickPhysicalDevice()
    {
        u32 DeviceCount;
        VkResult Result;
        Result = vkEnumeratePhysicalDevices(Lunar::vk::getInstance(), &DeviceCount, NULL);
        if (Result != VK_SUCCESS) {
            Lunar::Error("Renderer> Failed to enumerate graphics cards.");
            Lunar::Exit();
        }

        std::vector<VkPhysicalDevice> Devices(DeviceCount);
        vkEnumeratePhysicalDevices(Lunar::vk::getInstance(), &DeviceCount, Devices.data());

        std::unordered_map<VkPhysicalDevice*, u32> Scores;
        for (auto& Device : Devices) {
            u32 Score = DeviceScore(Device, m_Surface, m_Extensions);
            if (Score != 0) {
                Scores.insert({ &Device, Score });
            }
        }

        if (Scores.empty()) {
            Lunar::Error("Renderer> No suitable GPU found.");
            Lunar::Exit();
        }

        std::pair<VkPhysicalDevice*, u32> Best = { 
            &Devices[0], 
            Scores[ &Devices[0] ] 
        };
        for (auto& Score : Scores) {
            if (Best.second < Score.second) {
                Best.first = Score.first;
                Best.second = Score.second;
            }
        }

        VkPhysicalDeviceProperties Properties;
        vkGetPhysicalDeviceProperties(*Best.first, &Properties);

        Lunar::Print("Renderer> Picked device {} with score {}.", Properties.deviceName, Best.second);
        m_GPU = *Best.first;
    }
}