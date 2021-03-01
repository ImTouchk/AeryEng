#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>
#include <vector>
#include <array>

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
        Aery::log(Aery::debug_format("<Vk::DebugCallback> {}", CallbackData->pMessage), fmt::color::aqua);
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

    std::vector<const char*> GetRequiredExtensions(bool UseLayers) {
        Aery::mut_u32 ExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&ExtensionCount);
        std::vector<const char*> Extensions(glfwExtensions, glfwExtensions + ExtensionCount);
        if (UseLayers) {
            Extensions.push_back(
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME
            );
        }
        return Extensions;
    }

    bool CheckLayerSupport(std::array<const char*, 1>& Layers) {
        Aery::mut_u32 LayerCount = 0;
        vk::Result Result = vk::enumerateInstanceLayerProperties(&LayerCount, nullptr);
        if (Result != vk::Result::eSuccess) {
            Aery::error("<CheckLayerSupport> EnumerateInstanceLayerProperties failed."); 
            return false;
        }

        std::vector<vk::LayerProperties> LProperties = std::vector<vk::LayerProperties>(LayerCount);
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
    
    bool CreateGPUBuffer(vk::BufferUsageFlags Flags, void* Data, vk::DeviceSize Size, vk::Device& Device, vk::CommandPool& Pool,
        VmaAllocator& Allocator, VmaAllocation& Allocation, vk::Queue& GraphicsQ, vk::Buffer& Buffer) {
        VkBufferCreateInfo BufferInfo = {
            .flags = static_cast<VkBufferUsageFlags>(Flags),
            .size = static_cast<VkDeviceSize>(Size),
        };

        VmaAllocationCreateInfo AllocationInfo = {
            .usage = VMA_MEMORY_USAGE_CPU_ONLY
        };

        VmaAllocation CPUAllocation;
        VkBuffer CPUBuffer;
        vmaCreateBuffer(Allocator, &BufferInfo, &AllocationInfo, &CPUBuffer, &CPUAllocation, nullptr);

        void* IndexData;
        vmaMapMemory(Allocator, CPUAllocation, &IndexData);
        memcpy(IndexData, Data, Size);
        vmaUnmapMemory(Allocator, CPUAllocation);

        AllocationInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        VmaAllocation TempAllocation;
        VkBuffer TempBuffer;
        vmaCreateBuffer(Allocator, &BufferInfo, &AllocationInfo, &TempBuffer, &TempAllocation, nullptr);

        vk::CommandBufferAllocateInfo CmdAllocInfo = {
            .commandPool = Pool,
            .level = vk::CommandBufferLevel::ePrimary,
            .commandBufferCount = 1,
        };

        vk::CommandBuffer CmdBuffer = Device.allocateCommandBuffers(CmdAllocInfo)[0];
        vk::CommandBufferBeginInfo CmdBeginInfo = {
            .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
        };
        CmdBuffer.begin(CmdBeginInfo);

        vk::BufferCopy CopyRegion = {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = BufferInfo.size,
        };
        CmdBuffer.copyBuffer(static_cast<vk::Buffer>(CPUBuffer), static_cast<vk::Buffer>(TempBuffer), 1, &CopyRegion);
        CmdBuffer.end();

        vk::SubmitInfo SubmitInfo = {
            .commandBufferCount = 1,
            .pCommandBuffers = &CmdBuffer
        };
        vk::Result Result = GraphicsQ.submit(1, &SubmitInfo, {});
        GraphicsQ.waitIdle();
        Device.freeCommandBuffers(Pool, 1, &CmdBuffer);

        Buffer = static_cast<vk::Buffer>(TempBuffer);
        Allocation = TempAllocation;

        vmaDestroyBuffer(Allocator, CPUBuffer, CPUAllocation);
        return true;
    }
}
}