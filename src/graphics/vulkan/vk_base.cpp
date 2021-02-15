#include "utils/debug.hpp"
#include "utils/types.hpp"
#include "graphics/window.hpp"
#include "graphics/vulkan/vk_common.hpp"
#include "graphics/vk_renderer.hpp"
#include <fmt/core.h>
#include <cassert>
#include <vector>
#include <mutex>

using namespace std;

static vector<Aery::Graphics::VkRenderer*> VkRenderers = {};
static Aery::mut_u16 Index = 0;
static mutex ListMutex = {};

namespace Aery { namespace Graphics {
    VkRenderer::VkRenderer() : m_ID(Index) {
        ListMutex.lock();
            VkRenderers.push_back(this);
            m_States.active = false;
            Index++;
        ListMutex.unlock();
    }

    VkRenderer::~VkRenderer() {
        ListMutex.lock();
            VkRenderers.erase(
                VkRenderers.begin() + m_ID
            );
        ListMutex.unlock();
        if (m_States.active) {
            destroy();
        }
    }

    bool VkRenderer::create(RendererCreateInfo& Input) {
        Aery::log(fmt::format("--------------- CREATING VULKAN RENDERER {} ---------------", m_ID), fmt::color::hot_pink);
        // One-time creation
        if (Input.window == nullptr) {
            Aery::error("<VkRenderer::create> VkRendererCreateInfo::window was NULL.");
            return false;
        }
        m_PresentMode = Input.present_mode;
        m_Window = Input.window; 

        m_Objects = vector<pair<bool, VkObject>>(8, { false, {} });
        m_Shaders = unordered_map<PShader, VkShader>();
        
        if (!glfwVulkanSupported()) { return false; }
        if (!CreateInstance()) { return false; }
        SetupDM(); // Program runs without it as well
        if (!CreateSurface()) { return false; }
        if (!PickPhysicalDevice()) { return false; }
        if (!CreateLogicalDevice()) { return false; }
        if (!CreateAllocator()) { return false; }
        if (!CreateDepthBuffer()) { return false; }

        // Might be recreated later
        if (!CreateSwapchain()) { return false; }
        if (!CreateImageViews()) { return false; }
        if (!CreateRenderPass()) { return false; }
        if (!CreateFramebuffers()) { return false; }
        if (!CreateCommandPool()) { return false; }
        if (!AllocateCommandBuffers()) { return false; }
        if (!CreateSyncObjects()) { return false; }

        m_Scissor = vk::Rect2D {
            .offset = { 0, 0 },
            .extent = m_Swapchain.extent
        };

        m_Viewport = vk::Viewport {
            .x = 0, .y = 0,
            .width = (f32)m_Swapchain.extent.width,
            .height = (f32)m_Swapchain.extent.height,
            .minDepth = 0.0f, .maxDepth = 1.0f
        };

        m_States.active = true;
        m_Window->_onVulkanCreated(*this);
        return true;
    }

    void VkRenderer::destroy() {
        Aery::log(fmt::format("--------------- DESTROYING VULKAN RENDERER {} ---------------", m_ID), fmt::color::hot_pink);
        DestroyObjects();
        DestroyShaders();

        DestroySyncObjects();
        DestroyCommandPool();
        DestroyFramebuffers();
        DestroyRenderPass();
        DestroyImageViews();
        DestroySwapchain();

        DestroyDepthBuffer();
        DestroyAllocator();
        DestroyLogicalDevice();
        DestroySurface();
        DestroyDM();
        DestroyInstance();
        m_States.active = 0;
    }

    void VkRenderer::_onResize() {
        Aery::log("--------------- RENDERER RESIZE EVENT ---------------", fmt::color::hot_pink);

        int Width = 0, Height = 0;
        glfwGetFramebufferSize(m_Window->info().handle, &Width, &Height);
        if (Width == 0 || Height == 0) {
            m_States.minimized = true;
            return;
        }

        m_Device.waitIdle();

        DestroyFramebuffers();
        DestroyImageViews();

        RecreateSwapchain();

        CreateImageViews();
        CreateFramebuffers();

        m_Scissor = vk::Rect2D {
            .offset = { 0, 0 },
            .extent = m_Swapchain.extent
        };

        m_Viewport = vk::Viewport {
            .x = 0, .y = 0,
            .width = (f32)m_Swapchain.extent.width,
            .height = (f32)m_Swapchain.extent.height,
            .minDepth = 0.0f, .maxDepth = 1.0f
        };
    }

    void VkRenderer::draw() {
        if (m_States.minimized) {
            int Width = 0, Height = 0;
            glfwGetFramebufferSize(m_Window->info().handle, &Width, &Height);
            if (Width == 0 || Height == 0) {
                return;
            }
            m_States.minimized = false;
            _onResize();
        }

        vk::Result Result = m_Device.waitForFences(1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

        mut_u32 ImageIndex = 0;
        Result = m_Device.acquireNextImageKHR(
            m_Swapchain.instance, 
            UINT64_MAX, 
            m_ImageAvailable[m_CurrentFrame], 
            {},
            &ImageIndex,
            vk::DispatchLoaderStatic()
        );
        CreateCommandBuffer(ImageIndex);

        if (m_ImagesInFlight[m_CurrentFrame] != VK_NULL_HANDLE) {
            Result = m_Device.waitForFences(1, &m_ImagesInFlight[m_CurrentFrame], VK_TRUE, UINT64_MAX);
        }
        m_ImagesInFlight[m_CurrentFrame] = m_InFlightFences[m_CurrentFrame];
        Result = m_Device.resetFences(1, &m_InFlightFences[m_CurrentFrame]);

        vk::Semaphore SignalSemaphores[] = { m_RenderFinished[m_CurrentFrame] };
        vk::Semaphore WaitSemaphores[] = { m_ImageAvailable[m_CurrentFrame] };
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

        Result = m_GraphicsQ.submit(1, &SubmitInfo, m_InFlightFences[m_CurrentFrame]);

        vk::SwapchainKHR Swapchains[] = { m_Swapchain.instance };
        vk::PresentInfoKHR PresentInfo = {
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = SignalSemaphores,
            .swapchainCount = 1,
            .pSwapchains = Swapchains,
            .pImageIndices = &ImageIndex,
            .pResults = nullptr
        };

        Result = m_PresentQ.presentKHR(PresentInfo);
        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
}
}