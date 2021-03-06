#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>
#include <unordered_map>
#include <vector>
#include <set>

static Aery::u32 GetDeviceScore(vk::PhysicalDevice& PhysDevice, vk::SurfaceKHR& Surface, const std::array<const char*, 1>& Extensions) {
    Aery::mut_u32 Score = 0;
    
    Aery::mut_u32 ExtensionCount = 0;
    vk::Result Result = PhysDevice.enumerateDeviceExtensionProperties(nullptr, &ExtensionCount, nullptr);
    if (Result != vk::Result::eSuccess) {
        return Score;
    }

    vk::ExtensionProperties* GPUExtensions = new vk::ExtensionProperties[ExtensionCount];
    Result = PhysDevice.enumerateDeviceExtensionProperties(nullptr, &ExtensionCount, GPUExtensions);
    if (Result != vk::Result::eSuccess) {
        return Score;
    }

    std::set<std::string> RequiredExt(Extensions.begin(), Extensions.end());
    for (Aery::mut_u32 i = 0; i < ExtensionCount; i++) {
        RequiredExt.erase(GPUExtensions[i].extensionName);
    }
    delete[] GPUExtensions;

    vk::PhysicalDeviceProperties Properties;
    PhysDevice.getProperties(&Properties);

    switch (Properties.deviceType) {
        default: Score += 1; break;
        case vk::PhysicalDeviceType::eCpu: Score += 1; break;
        case vk::PhysicalDeviceType::eIntegratedGpu: Score += 5; break;
        case vk::PhysicalDeviceType::eDiscreteGpu: Score += 10; break;
    }

    vk::PhysicalDeviceMemoryProperties MemoryProps;
    PhysDevice.getMemoryProperties(&MemoryProps);
    for (Aery::mut_u32 i = 0; i < MemoryProps.memoryHeapCount; i++) {
        if (MemoryProps.memoryHeaps[i].flags & vk::MemoryHeapFlagBits::eDeviceLocal) {
            Aery::u32 Size = Aery::u32( MemoryProps.memoryHeaps[i].size ) / 1024 / 1024;
            Score += Size;
        }
    }
    
    Aery::Graphics::VkQueueFamilyIndices Indices = Aery::Graphics::FindQueueFamilies(PhysDevice, Surface);
    Aery::Graphics::VkSwapchainSupportDetails Support = Aery::Graphics::QuerySwapSupport(PhysDevice, Surface);
    bool SwapAdequate = !Support.formats.empty() && !Support.presentModes.empty();
    bool Adequate = Indices.isComplete() && SwapAdequate;
    if (!Adequate) {
        Score = 0;
    }
    return Score;
}

namespace Aery { namespace Graphics {
    bool VkRenderer::PickPhysicalDevice() {
        mut_u32 DeviceCount = 0;
        vk::Result Result = m_Instance.enumeratePhysicalDevices(&DeviceCount, nullptr);
        if (Result != vk::Result::eSuccess) {
            Aery::error("<VkRenderer::PickPhysicalDevice> Failed to enumerate physical devices.");
            return false;
        }
        std::vector<vk::PhysicalDevice> Devices(DeviceCount);
        Result = m_Instance.enumeratePhysicalDevices(&DeviceCount, Devices.data());
        if (Result != vk::Result::eSuccess) {
            Aery::error("<VkRenderer::PickPhysicalDevice> Failed to enumerate physical devices.");
            return false;
        }

        std::unordered_map<vk::PhysicalDevice*, mut_u32> Scores = {};
        for (mut_u32 i = 0; i < Devices.size(); i++) {
            u32 Score = GetDeviceScore(Devices[i], m_Surface, m_Extensions);
            if (Score != 0) { Scores[&Devices[i]] = Score; }
        }
        if (Scores.empty()) {
            Aery::error("<VkRenderer::PickPhysicalDevice> No user GPU is capable.");
            return false;
        }

        std::pair<vk::PhysicalDevice*, mut_u32> Best = { &Devices[0], Scores[&Devices[0]] };
        for (std::pair<vk::PhysicalDevice*, u32> Score : Scores) {
            if (Best.second < Score.second) {
                Best.first = Score.first;
                Best.second = Score.second;
            }
        }

        vk::PhysicalDeviceProperties Properties;
        Best.first->getProperties(&Properties);

        Aery::log(debug_format("<VkRenderer::PickPhysicalDevice> ID {} picked device {} with score {}.", m_ID, Properties.deviceName, Best.second));
        m_PhysicalDevice = *Best.first;
        return true;
    }
}
}