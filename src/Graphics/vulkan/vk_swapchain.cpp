#include <vulkan/vulkan.h>

#include "Debug.h"
#include "Types.h"
#include "Graphics/VkCommon.h"
#include "Graphics/Renderer.h"

namespace {
    VkSurfaceFormatKHR& PickSurfaceFormat(std::vector<VkSurfaceFormatKHR>& Formats)
    {
        if (Formats.size() == 0) {
            Lunar::Error("Renderer> No surface formats available.");
            Lunar::Exit();
            Formats.push_back({});
            return Formats[0];
        }

        for (auto& SurfaceFormat : Formats) {
            if (SurfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                SurfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
            ) return SurfaceFormat;
        }

        return Formats[0];
    }
    
    VkPresentModeKHR& PickPresentMode(std::vector<VkPresentModeKHR>& Modes)
    {
        if (Modes.size() == 0) {
            Lunar::Error("Renderer> No present modes available.");
            Lunar::Exit();
            Modes.push_back({});
            return Modes[0];
        }

        for (auto& PresentMode : Modes) {
            if (PresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                return PresentMode;
        }

        Modes.push_back(VK_PRESENT_MODE_IMMEDIATE_KHR);
        return Modes[ Modes.size() ];
    }

    VkExtent2D PickExtent(Lunar::Window& Window, const VkSurfaceCapabilitiesKHR& Capabilities)
    {
        if (Capabilities.currentExtent.width != UINT32_MAX &&
            Capabilities.currentExtent.width != 0) {
            return Capabilities.currentExtent;
        }

        VkExtent2D ActualExtent = {
            .width = Window.width(),
            .height = Window.height()
        };

        ActualExtent.width = std::max(Capabilities.minImageExtent.width, ActualExtent.width);
        ActualExtent.height = std::max(Capabilities.minImageExtent.height, ActualExtent.height);
        return ActualExtent;
    }
}

namespace Lunar {
    void Renderer::CreateImageViews()
    {
        m_SwapViews.resize(m_SwapImages.size());

        for (u32 i : range( m_SwapImages.size() )) {
            VkImageViewCreateInfo ViewCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image = m_SwapImages[i],
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = m_SwapFormat,
                .components = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                },
                .subresourceRange = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
                },
            };

            VkResult Result;
            Result = vkCreateImageView(m_Device, &ViewCreateInfo, NULL, &m_SwapViews[i]);
            if (Result != VK_SUCCESS) {
                Lunar::Error("Renderer> Failed to create image view {}.", i);
                Lunar::Exit();
            }
        }

        Lunar::Print("Renderer> Created {} image views.", m_SwapImages.size());
    }

    void Renderer::DestroyImageViews()
    {
        if (m_Device == NULL) {
            return;
        }

        for (auto& View : m_SwapViews) {
            if (!View) {
                continue;
            }
            vkDestroyImageView(m_Device, View, nullptr);
        }
    }

    void Renderer::CreateSwapchain()
    {
        vk::swapchainSupportDetails& Details = vk::querySwapSupport(m_GPU, m_Surface);
        VkSurfaceFormatKHR& Format = PickSurfaceFormat(Details.formats);
        VkPresentModeKHR& PresentMode = PickPresentMode(Details.presentModes);
        VkExtent2D Extent = PickExtent(*m_Window, Details.capabilities);
        auto& Capabilities = Details.capabilities;
        u32 ImageCount = Capabilities.minImageCount + 1;

        if (Capabilities.maxImageCount > 0 && ImageCount > Capabilities.maxImageCount) {
            ImageCount = Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR SwapCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = m_Surface,
            .minImageCount = ImageCount,
            .imageFormat = Format.format,
            .imageColorSpace = Format.colorSpace,
            .imageExtent = Extent,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .preTransform = Details.capabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = PresentMode,
            .clipped = VK_TRUE,
            .oldSwapchain = nullptr
        };

        const vk::queueFamilyIndices& Indices = vk::findQueueFamilies(m_GPU, m_Surface);
        u32 IndicesArray[] = {
            Indices.gFamily.value(),
            Indices.pFamily.value()
        };

        if (Indices.gFamily != Indices.pFamily) {
            SwapCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            SwapCreateInfo.queueFamilyIndexCount = 2;
            SwapCreateInfo.pQueueFamilyIndices = IndicesArray;
        } else {
            SwapCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            SwapCreateInfo.queueFamilyIndexCount = 1;
            SwapCreateInfo.pQueueFamilyIndices = NULL;
        }

        VkResult Result;
        Result = vkCreateSwapchainKHR(m_Device, &SwapCreateInfo, nullptr, &m_Swapchain);
        if (Result != VK_SUCCESS) {
            Lunar::Error("Renderer> Failed to create a swapchain.");
            Lunar::Exit();
        }

        Result = vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &ImageCount, NULL);
        if (Result != VK_SUCCESS) {
            Lunar::Error("Renderer> Failed to acquire the swapchain images.");
            Lunar::Exit();
        }

        m_SwapImages.resize(ImageCount);
        vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &ImageCount, m_SwapImages.data());

        m_SwapFormat = Format.format;
        m_SwapExtent = Extent;

        Lunar::Print("Renderer> Swapchain created.");
    }

    void Renderer::DestroySwapchain()
    {
        if (m_Swapchain == NULL || m_Device == NULL) {
            return;
        }

        vkDeviceWaitIdle(m_Device);
        vkDestroySwapchainKHR(m_Device, m_Swapchain, NULL);
        Lunar::Print("Renderer> Swapchain destroyed.");
    }
}