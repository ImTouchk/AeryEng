#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

using namespace std;

namespace {
    vk::Format FindSupportedFormat(vk::PhysicalDevice& GPU, const std::vector<vk::Format>& Candidates, vk::ImageTiling Tiling, vk::FormatFeatureFlags Flags) {
        for (auto& Format : Candidates) {
            vk::FormatProperties Properties;
            GPU.getFormatProperties(Format, &Properties);
            if (Tiling == vk::ImageTiling::eLinear && (Properties.linearTilingFeatures & Flags) == Flags) {
                return Format;
            }
            else if (Tiling == vk::ImageTiling::eOptimal && (Properties.optimalTilingFeatures & Flags) == Flags) {
                return Format;
            }
        }
    }

    vk::Format FindDepthFormat(vk::PhysicalDevice& GPU) {
        return FindSupportedFormat(GPU,
            { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );
    }

    bool HasStencilComponent(vk::Format Format) {
        return Format == vk::Format::eD32SfloatS8Uint || Format == vk::Format::eD24UnormS8Uint;
    }
}

namespace Aery { namespace Graphics {
    bool VkRenderer::CreateDepthBuffer() {
        vk::Format DepthFormat = FindDepthFormat(m_PhysicalDevice);
        VkImageCreateInfo ImageInfo = {
            .imageType = VK_IMAGE_TYPE_2D,
            .format = static_cast<VkFormat>(DepthFormat),
            .extent = {
                .width = m_Swapchain.extent.width,
                .height = m_Swapchain.extent.height,
                .depth = 1
            },
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };

        VmaAllocationCreateInfo AllocationInfo = {
            .usage = VMA_MEMORY_USAGE_CPU_ONLY
        };

        VkImage DepthImage;
        vmaCreateImage(m_Allocator, &ImageInfo, &AllocationInfo, &DepthImage, &m_Depth.allocation, nullptr);
        m_Depth.image = static_cast<vk::Image>(DepthImage);
        return true;
    }

    void VkRenderer::DestroyDepthBuffer() {

    }
}
}