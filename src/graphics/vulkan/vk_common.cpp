#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>
#include <vector>
#include <array>

using namespace std;

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback
(
    VkDebugUtilsMessageSeverityFlagBitsEXT Severity,
    VkDebugUtilsMessageTypeFlagsEXT Type,
    const VkDebugUtilsMessengerCallbackDataEXT* CallbackData,
    void* UserData
)
{   
    if (Severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ||
        Severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        Aery::log(fmt::format("<Vk::DebugCallback> {}", CallbackData->pMessage), fmt::color::aqua);
    }

    return VK_FALSE;
}


namespace Aery { namespace Graphics {
    vk::DebugUtilsMessengerCreateInfoEXT EmptyDMCInfo() {
        using SFlags = vk::DebugUtilsMessageSeverityFlagBitsEXT;
        using MTFlags = vk::DebugUtilsMessageTypeFlagBitsEXT;
        vk::DebugUtilsMessengerCreateInfoEXT CreateInfo = {
            .messageSeverity = SFlags::eVerbose | SFlags::eError,
            .messageType = MTFlags::eGeneral | MTFlags::eValidation | MTFlags::ePerformance,
            .pfnUserCallback = DebugCallback
        };
        return CreateInfo;
    }

    // This function is called on every resize event, therefore caching the result will
    // definitely improve performance
    VkQueueFamilyIndices& FindQueueFamilies(vk::PhysicalDevice& GPU, vk::SurfaceKHR& Surface) {
        static VkQueueFamilyIndices CachedIndices = {};
        static vk::PhysicalDevice& CachedGPU = GPU;
        static bool Checked = false;

        if (Checked && CachedGPU == GPU) {
            Aery::log("<FindQueueFamilies> Using cached result.");
            return CachedIndices;
        }
        
        CachedGPU = GPU;

        mut_u32 FamilyCount = 0;
        GPU.getQueueFamilyProperties(&FamilyCount, nullptr);
        vk::QueueFamilyProperties* QFamilies = new vk::QueueFamilyProperties[FamilyCount];
        GPU.getQueueFamilyProperties(&FamilyCount, QFamilies);
        
        for (mut_u32 i = 0; i < FamilyCount; i++) {
            VkBool32 PresentSupport = false;
            vk::Result Result = GPU.getSurfaceSupportKHR(i, Surface, &PresentSupport);
            if (PresentSupport) { CachedIndices.pFamily = i; }
            if (QFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) { CachedIndices.gFamily = i; }
            if (CachedIndices.isComplete()) { break; }
        }
        
        delete[] QFamilies;
        Checked = true;

        return CachedIndices;
    }

    VkSwapchainSupportDetails& QuerySwapSupport(vk::PhysicalDevice& GPU, vk::SurfaceKHR& Surface) {
        static VkSwapchainSupportDetails CachedDetails = {};
        static vk::PhysicalDevice& CachedGPU = GPU;
        static bool Checked = false;

        if (Checked && CachedGPU == GPU) {
            Aery::log("<QuerySwapSupport> Using cached result.");
            return CachedDetails;
        }

        vk::Result Result = GPU.getSurfaceCapabilitiesKHR(Surface, &CachedDetails.capabilities);

        mut_u32 FormatCount = 0;
        Result = GPU.getSurfaceFormatsKHR(Surface, &FormatCount, nullptr);
        if (FormatCount != 0) {
            CachedDetails.formats.resize(FormatCount);
            Result = GPU.getSurfaceFormatsKHR(Surface, &FormatCount, CachedDetails.formats.data());
        }

        mut_u32 PModeCount = 0;
        Result = GPU.getSurfacePresentModesKHR(Surface, &PModeCount, nullptr);
        if (PModeCount != 0) {
            CachedDetails.presentModes.resize(PModeCount);
            Result = GPU.getSurfacePresentModesKHR(Surface, &PModeCount, CachedDetails.presentModes.data());
        }
        return CachedDetails;
    }

    vector<const char*> GetRequiredExtensions(bool UseLayers) {
        Aery::mut_u32 ExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&ExtensionCount);
        vector<const char*> Extensions(glfwExtensions, glfwExtensions + ExtensionCount);
        if (UseLayers) {
            Extensions.push_back(
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME
            );
        }
        return Extensions;
    }

    bool CheckLayerSupport(vector<const char*>& Layers) {
        Aery::mut_u32 LayerCount = 0;
        vk::Result Result = vk::enumerateInstanceLayerProperties(&LayerCount, nullptr);
        if (Result != vk::Result::eSuccess) {
            Aery::error("<CheckLayerSupport> EnumerateInstanceLayerProperties failed."); 
            return false;
        }

        vector<vk::LayerProperties> LProperties = vector<vk::LayerProperties>(LayerCount);
        Result = vk::enumerateInstanceLayerProperties(&LayerCount, LProperties.data());

        for (Aery::mut_u32 i = 0; i < Aery::mut_u32(Layers.size()); i++) {
            bool LayerFound = false;

            for (Aery::mut_u32 j = 0; j < Aery::mut_u32(Layers.size()); j++) {
                if (strcmp(Layers[i], LProperties[j].layerName) == 0) {
                    LayerFound = true;
                    break;
                }
            }

            if (!LayerFound) {
                return false;
            }
        }
        return true;
    }
}
}