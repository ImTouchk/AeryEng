#pragma once

#include "utils/types.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h> 
#include <unordered_map>
#include <vector>

namespace Aery {
    using PVkShader = mut_u32;
    using PVkObject = mut_u32;

    class Window;
    class VkObject;
    struct VkShader;
    struct VkShaderCreateInfo;
    struct VkObjectCreateInfo;
    
    enum VkRendererCreateFlags {
        VK_RENDERER_VSYNC = 2,
        VK_RENDERER_TRIPLE_BUFFERING = 4,
        VK_RENDERER_UNCAPPED = 5
    };

    struct VkRendererCreateInfo {
        Window* window = nullptr;
        mut_u16 render_mode = VK_RENDERER_VSYNC;
    };

    class VkRenderer {
    public:
        VkRenderer(); ~VkRenderer();

        bool create(VkRendererCreateInfo&&) = delete;
        bool create(VkRendererCreateInfo&);
        void destroy();
        void draw();
        
        // Object methods

        VkObject& getObjectByID(u32);
        bool createDefaultObject(PVkObject*);
        bool createObject(VkObjectCreateInfo&&, PVkObject*) = delete;
        bool createObject(VkObjectCreateInfo&, PVkObject*);
        void destroyObject(PVkObject);

        // Shader methods

        VkShader& getShaderByID(u32);
        bool createDefaultShader(PVkShader*);
        bool createShader(VkShaderCreateInfo&&, PVkShader*) = delete;
        bool createShader(VkShaderCreateInfo&, PVkShader*);
        void destroyShader(PVkShader);

        // Events

        void _onResize();
    private:
        // One-time creation

        bool CreateInstance();      void DestroyInstance();
        bool SetupDM();             void DestroyDM();
        bool CreateSurface();       void DestroySurface();
        bool PickPhysicalDevice();
        bool CreateLogicalDevice(); void DestroyLogicalDevice();
        bool CreateAllocator();     void DestroyAllocator();
        void DestroyShaders();      void DestroyObjects();

        // On-resize creation

        bool RecreateSwapchain();
        bool CreateSwapchain(bool = false);     void DestroySwapchain();
        bool CreateImageViews();    void DestroyImageViews();
        bool CreateRenderPass();    void DestroyRenderPass();
        bool CreateFramebuffers();  void DestroyFramebuffers();
        bool CreateCommandPool();   void DestroyCommandPool();
        bool CreateSyncObjects();   void DestroySyncObjects();
        bool AllocateCommandBuffers();

        // On-draw creation

        bool CreateCommandBuffer(int);

        // Utils

        u32 FindMemoryType(u32, vk::MemoryPropertyFlags);

        // Variables

        Window* m_Window = nullptr;
        bool m_LayersUsed = false;
        bool m_Minimized = false;
        bool m_UseLayers = true;
        bool m_Active = false;
        u32 m_ID = -1;

        mut_u16 m_RenderMode = VK_RENDERER_VSYNC;

        vk::Instance m_Instance;
        vk::DebugUtilsMessengerEXT m_DebugMessenger;
        vk::SurfaceKHR m_Surface;
        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;
        vk::Queue m_GraphicsQ;
        vk::Queue m_PresentQ;

        VmaAllocator m_Allocator;
        
        struct {
            vk::SwapchainKHR instance;
            vk::Extent2D extent;
            vk::Format format;
            std::vector<vk::Image> images = {};
            std::vector<vk::ImageView> views = {};
            std::vector<vk::Framebuffer> buffers = {};
        } m_Swapchain = {};

        vk::RenderPass m_RenderPass;
        vk::CommandPool m_CommandPool;

        std::vector<vk::CommandBuffer> m_CommandBuffers = {};
        bool m_CmdBuffersCreated = false;

        vk::Viewport m_Viewport;
        vk::Rect2D m_Scissor;

        std::vector<vk::Semaphore> m_ImageAvailable = {};
        std::vector<vk::Semaphore> m_RenderFinished = {};
        std::vector<vk::Fence> m_ImagesInFlight = {};
        std::vector<vk::Fence> m_InFlightFences = {};
        mut_u16 m_CurrentFrame = 0;

        u32 MAX_FRAMES_IN_FLIGHT = 2;

        std::vector<const char*> m_Extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        std::vector<const char*> m_Layers = {
            "VK_LAYER_KHRONOS_validation"
        };

        std::unordered_map<mut_u32, VkShader> m_Shaders = {};
        std::unordered_map<mut_u32, VkObject> m_Objects = {};
    };
}