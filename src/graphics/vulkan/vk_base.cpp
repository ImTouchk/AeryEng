#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/window.hpp"
#include "graphics/vk_shader.hpp"
#include "graphics/vk_object.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <fmt/core.h>
#include <vector>

using namespace std;

static vector<Aery::VkRenderer*> VkRenderers = {};
static Aery::mut_u32 Index = 0;

namespace Aery {
    VkRenderer::VkRenderer() : m_ID(Index) {
        VkRenderers.push_back(this);
        m_Active = false;
        Index++;
    }

    VkRenderer::~VkRenderer() {
        VkRenderers.erase(
            VkRenderers.begin() + m_ID
        );
        if (m_Active) {
            destroy();
        }
    }

    bool VkRenderer::create(Window& Surface) {
        Aery::log(fmt::format("--------------- CREATING VULKAN RENDERER {} ---------------", m_ID), fmt::color::hot_pink);
        // One-time creation
        m_Window = &Surface; m_Window->_onRendererCreated(*this);
        if (!glfwVulkanSupported()) { return false; }
        if (!CreateInstance()) { return false; }
        SetupDM(); // Program runs without it as well
        if (!CreateSurface()) { return false; }
        if (!PickPhysicalDevice()) { return false; }
        if (!CreateLogicalDevice()) { return false; }

        // Might be recreated later
        if (!CreateSwapchain()) { return false; }
        if (!CreateImageViews()) { return false; }
        if (!CreateRenderPass()) { return false; }
        if (!CreateFramebuffers()) { return false; }
        if (!CreateCommandPool()) { return false; }
        if (!AllocateCommandBuffers()) { return false; }
        if (!CreateSemaphores()) { return false; }
        return true;
    }

    void VkRenderer::destroy() {
        Aery::log(fmt::format("--------------- DESTROYING VULKAN RENDERER {} ---------------", m_ID), fmt::color::hot_pink);
        DestroyShaders();
        DestroySemaphores();
        DestroyCommandPool();
        DestroyFramebuffers();
        DestroyRenderPass();
        DestroyImageViews();
        DestroySwapchain();
        DestroyLogicalDevice();
        DestroySurface();
        DestroyDM();
        DestroyInstance();
    }

    void VkRenderer::_onResize() {
        Aery::log("--------------- RESIZE EVENT ---------------", fmt::color::hot_pink);
        m_Device.waitIdle();

        if (m_Window->width() == 0 || m_Window->height() == 0) {
            m_Minimized = true;
            return;
        }
        // TO DO
    }

    void VkRenderer::DestroyShaders() {
        for (mut_u32 i = 0; i < m_Shaders.size(); i++) {
            destroyShader(m_Shaders[i]);
        }
        m_Shaders.clear();
    }

    void VkRenderer::draw() {
        if (m_Minimized) {
            int Width = 0, Height = 0;
            glfwGetFramebufferSize(m_Window->info().handle, &Width, &Height);
            if (Width == 0 || Height == 0) {
                return;
            }
            m_Minimized = false;
            _onResize();
        }

        vk::Fence Fence = {};
        mut_u32 ImageIndex = 0;
        m_Device.acquireNextImageKHR(m_Swapchain.instance, UINT64_MAX, m_ImageAvailable, Fence, &ImageIndex, vk::DispatchLoaderStatic());
        CreateCommandBuffer(ImageIndex);

        vk::Semaphore SignalSemaphores[] = { m_RenderFinished };
        vk::Semaphore WaitSemaphores[] = { m_ImageAvailable };
        vk::PipelineStageFlags WaitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
        vk::SubmitInfo SubmitInfo = {
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = WaitSemaphores,
            .pWaitDstStageMask = WaitStages,
            .commandBufferCount = 1,
            .pCommandBuffers = &m_CommandBuffers[ImageIndex],
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = SignalSemaphores
        };

        m_GraphicsQ.submit(1, &SubmitInfo, Fence);

        vk::SwapchainKHR Swapchains[] = { m_Swapchain.instance };
        vk::PresentInfoKHR PresentInfo = {
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = SignalSemaphores,
            .swapchainCount = 1,
            .pSwapchains = Swapchains,
            .pImageIndices = &ImageIndex,
            .pResults = nullptr
        };

        m_PresentQ.presentKHR(PresentInfo);
        Aery::log("Drawing...", fmt::color::aqua); // <--- wtf the window is unresponsive if i comment this out
    }
}