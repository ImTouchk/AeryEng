#pragma once

#include "utils/types.hpp"
#include "graphics/rn_common.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h> 
#include <unordered_map>
#include <vector>

namespace Aery { namespace Graphics {
    class VkShader : public Shader { public:
        vk::PipelineLayout layout;
        vk::PipelineCache cache;
        vk::Pipeline pipeline;
    };

    class VkObject : public Object { public:
        struct ObjectBuffer {
            VmaAllocation allocation;
            vk::Buffer buffer;
        } vertex, index;
    };

    class VkRenderer {
    public:
        VkRenderer(); ~VkRenderer();

        bool create(RendererCreateInfo&&) = delete;
        bool create(RendererCreateInfo&);
        void destroy();
        void draw();
        
        // Object methods

        bool createDefaultObject(PObject*);
        bool createObject(ObjectCreateInfo&&, PObject*) = delete;
        bool createObject(ObjectCreateInfo&, PObject*);
        void destroyObject(PObject);

        // Shader methods

        bool createDefaultShader(PShader*);
        bool createShader(ShaderCreateInfo&&, PShader*) = delete;
        bool createShader(ShaderCreateInfo&, PShader*);
        void destroyShader(PShader);

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

        struct {
            char useLayers : 1;
            char layersUsed : 1;
            char minimized : 1;
            char active : 1;
        } m_States;
        PresentMode m_PresentMode = PresentMode::eAny;
        Window* m_Window = nullptr;
        mut_u16 m_ID = -1;

        std::unordered_map<mut_u16, VkShader> m_Shaders = {};
        std::unordered_map<mut_u16, VkObject> m_Objects = {};

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

        u16 MAX_FRAMES_IN_FLIGHT = 2;

        std::vector<const char*> m_Extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        std::vector<const char*> m_Layers = {
            "VK_LAYER_KHRONOS_validation"
        };
    };
}
}