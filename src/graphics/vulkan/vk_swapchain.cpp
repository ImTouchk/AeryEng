#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <fmt/core.h>

static vk::SurfaceFormatKHR& PickSurfaceFormat(std::vector<vk::SurfaceFormatKHR>& Formats) {
    if (Formats.size() == 0) {
        Aery::error("<PickSurfaceFormat> No formats available.");
        Formats.push_back({});
        return Formats[0];
    }
    for (Aery::mut_u32 i = 0; i < Formats.size(); i++) {
        if (   Formats[i].format == vk::Format::eB8G8R8A8Srgb
            && Formats[i].colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return Formats[i];
        }
    }
    return Formats[0];
}

static vk::PresentModeKHR PickPresentMode(std::vector<vk::PresentModeKHR>& Modes, Aery::Graphics::PresentMode Preferred) {
    if (Modes.size() == 0) {
        Aery::error("<PickPresentMode> No present modes available.");
        return {};
    }

    vk::PresentModeKHR PresentMode = {};
    using namespace Aery::Graphics;
    switch (Preferred) {
        case PresentMode::eVsync: PresentMode = vk::PresentModeKHR::eFifo; break;
        case PresentMode::eTripleBuffering: PresentMode = vk::PresentModeKHR::eMailbox; break;
        case PresentMode::eImmediate: return vk::PresentModeKHR::eImmediate; break;
        default: PresentMode = vk::PresentModeKHR::eMailbox; break;
    }

    for (Aery::mut_u16 i = 0; i < Modes.size(); i++) {
        if (Modes[i] == PresentMode) {
            return Modes[i];
        }
    }

    Aery::warn("<PickPresentMode> Requested mode is not available. Using immediate instead.");
    return vk::PresentModeKHR::eImmediate;
}

static vk::Extent2D PickExtent(const Aery::Graphics::Window& Surface, const vk::SurfaceCapabilitiesKHR& Capabilities) {
    if (Capabilities.currentExtent.width != UINT32_MAX &&
        Capabilities.currentExtent.width != 0) {
        return Capabilities.currentExtent;
    }

    vk::Extent2D ActualExtent = {
        Surface.width(),
        Surface.height()
    };

    ActualExtent.width = std::max(Capabilities.minImageExtent.width, ActualExtent.width);
    ActualExtent.height = std::max(Capabilities.minImageExtent.height, ActualExtent.height);
    return ActualExtent;
}

namespace Aery { namespace Graphics {
    bool VkRenderer::CreateSwapchain() {
        VkSwapchainSupportDetails Support = QuerySwapSupport(m_PhysicalDevice, m_Surface);
        vk::SurfaceFormatKHR Format = PickSurfaceFormat(Support.formats);
        vk::PresentModeKHR PresentMode = PickPresentMode(Support.presentModes, m_PresentMode);
        vk::Extent2D Extent = PickExtent(*m_Window, Support.capabilities);
        mut_u32 ImageCount = Support.capabilities.minImageCount + 1;
        if (Support.capabilities.maxImageCount > 0 && ImageCount > Support.capabilities.maxImageCount) {
            ImageCount = Support.capabilities.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR SwapInfo = {
            .surface = m_Surface,
            .minImageCount = ImageCount,
            .imageFormat = Format.format,
            .imageColorSpace = Format.colorSpace,
            .imageExtent = Extent,
            .imageArrayLayers = 1,
            .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
            .preTransform = Support.capabilities.currentTransform,
            .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
            .presentMode = PresentMode,
            .clipped = VK_TRUE,
            .oldSwapchain = nullptr
        };
        
        const VkQueueFamilyIndices Indices = FindQueueFamilies(m_PhysicalDevice, m_Surface);
        const u32 FamilyIndices[] = {
            Indices.gFamily.value(),
            Indices.pFamily.value()
        };

        if (Indices.gFamily != Indices.pFamily) {
            SwapInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            SwapInfo.pQueueFamilyIndices = FamilyIndices;
            SwapInfo.queueFamilyIndexCount = 2;
        } else {
            SwapInfo.imageSharingMode = vk::SharingMode::eExclusive;
            SwapInfo.pQueueFamilyIndices = nullptr;
            SwapInfo.queueFamilyIndexCount = 0;
        }

        vk::Result Result = m_Device.createSwapchainKHR(&SwapInfo, nullptr, &m_Swapchain.instance);
        if (Result != vk::Result::eSuccess) {
            Aery::error(debug_format("<VkRenderer::CreateSwapchain> ID {} failed to create a swapchain.", m_ID));
            return false;
        }

        Result = m_Device.getSwapchainImagesKHR(m_Swapchain.instance, &ImageCount, nullptr);
        if (Result != vk::Result::eSuccess) {
            GetImageFail:
            Aery::error(debug_format("<VkRenderer::CreateSwapchain> ID {} failed to get the swapchain images.", m_ID));
            return false;
        }
        m_Swapchain.images.resize(ImageCount);
        Result = m_Device.getSwapchainImagesKHR(m_Swapchain.instance, &ImageCount, m_Swapchain.images.data());
        if (Result != vk::Result::eSuccess) {
            goto GetImageFail;
        }
        m_Swapchain.format = Format.format;
        m_Swapchain.extent = Extent;

        Aery::log(debug_format("<VkRenderer::CreateSwapchain> ID {} created a swapchain.", m_ID), fmt::color::light_green);
        return true;
    }

    void VkRenderer::DestroySwapchain() {
        m_Device.waitIdle();
        m_Device.destroySwapchainKHR(m_Swapchain.instance);
    }

    bool VkRenderer::RecreateSwapchain() {
        DestroySwapchain();
        return CreateSwapchain();
    }

    bool VkRenderer::CreateImageViews() {
        m_Swapchain.views.resize(
            m_Swapchain.images.size()
        );
        for (mut_u16 i = 0; i < m_Swapchain.images.size(); i++) {
            vk::ImageViewCreateInfo ViewInfo = {
                .image = m_Swapchain.images[i],
                .viewType = vk::ImageViewType::e2D,
                .format = m_Swapchain.format,
                .components = {
                    vk::ComponentSwizzle::eIdentity,
                    vk::ComponentSwizzle::eIdentity,
                    vk::ComponentSwizzle::eIdentity,
                },
                .subresourceRange = {
                    .aspectMask = vk::ImageAspectFlagBits::eColor,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1
                },
            };
            vk::Result Result = m_Device.createImageView(&ViewInfo, nullptr, &m_Swapchain.views[i]);
            if (Result != vk::Result::eSuccess) {
                Aery::error(debug_format("<VkRenderer::CreateImageViews> ID {} failed to create an image view {}.", m_ID, i));
                return false;
            }
        }

        Aery::log(debug_format("<VkRenderer::CreateImageViews> ID {} created all image views.", m_ID), fmt::color::light_green);
        return true;
    }

    void VkRenderer::DestroyImageViews() {
        for (mut_u16 i = 0; i < m_Swapchain.views.size(); i++) {
            if (!m_Swapchain.views[i]) {
                continue;
            }
            m_Device.destroyImageView(m_Swapchain.views[i], nullptr);
        }
        Aery::log(debug_format("<VkRenderer::DestroyImageViews> Destroyed all views.", m_ID));
    }
}
}