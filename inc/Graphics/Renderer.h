#ifndef LUNAR_RENDERER_H
#define LUNAR_RENDERER_H

#include "Graphics/Window.h"
#include "Graphics/VkCommon.h"
#include <array>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <vector>

namespace Lunar {
    class Renderer {
    public:
        Renderer() = default;
        ~Renderer() = default;

        void start(Window& CreateInfo);
        void stop();

        bool active() const;
        void draw();

        using ExtensionArray = std::array<const char*, 1>;
        using LayerArray = std::array<const char*, 1>;

    private:
        /* Permanent Resources */
        void PickPhysicalDevice();
        
        void SetupMessenger();  void DestroyMessenger();
        void CreateSurface();   void DestroySurface();
        void CreateDevice();    void DestroyDevice();
        void CreateAllocator(); void DestroyAllocator();

        void CreatePermanentResources();
        void DestroyPermanentResources();

        /* Temporary Resources */
        void OnResize();
        
        void CreateTemporaryResources();
        void DestroyTemporaryResources();

        void CreateSwapchain();    void DestroySwapchain();
        void CreateImageViews();   void DestroyImageViews();
        void CreateRenderPass();   void DestroyRenderPass();
        void CreateFramebuffers(); void DestroyFramebuffers();
        void CreateCommandPool();  void DestroyCommandPool();
        void CreateSyncObjects();  void DestroySyncObjects();
        void AllocateCommandBuffers();

        friend class Window;
        friend void _OnResize(GLFWwindow* Handle, int Width, int Height);
    private:
        Window* m_Window = NULL;
        bool m_Active    = false;
        u32 m_ID = 0;

        VkDebugUtilsMessengerEXT m_Messenger = NULL;
        VmaAllocator m_Allocator = NULL;
        VkSurfaceKHR m_Surface = NULL;
        VkPhysicalDevice m_GPU = NULL;
        VkDevice m_Device   = NULL;
        VkQueue m_GraphicsQ = NULL;
        VkQueue m_PresentQ  = NULL;

        VkSwapchainKHR m_Swapchain = NULL;
        VkExtent2D m_SwapExtent;
        VkFormat m_SwapFormat;
        std::vector<VkImage> m_SwapImages;
        std::vector<VkImageView> m_SwapViews;
        std::vector<VkFramebuffer> m_SwapBuffers;

        VkRenderPass m_RenderPass;

        VkCommandPool m_CommandPool;
        std::vector<VkCommandBuffer> m_CommandBuffers;
        bool m_CommandBuffersCreated = false;

        VkViewport m_Viewport;
        VkRect2D m_Scissor;

        std::vector<VkSemaphore> m_ImageAvailable;
        std::vector<VkSemaphore> m_RenderFinished;
        std::vector<VkFence> m_ImagesInFlight;
        std::vector<VkFence> m_InFlightFences;

        u32 m_CurrentFrame = 0;
        const u32 MAX_FRAMES_IN_FLIGHT = 2;

        ExtensionArray m_Extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        LayerArray m_Layers = {
            "VK_LAYER_KHRONOS_validation",
        };
    };
}

#endif // LUNAR_RENDERER_H