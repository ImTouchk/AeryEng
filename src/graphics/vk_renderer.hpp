#pragma once

#include "utils/types.hpp"
#include "graphics/rn_common.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h> 
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>

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
        
        bool createObject(ObjectCreateInfo&&, PObject*) = delete;
        bool createObject(ObjectCreateInfo&, PObject*);
        void bindPushConstant(PObject, void*);
        void destroyObject(PObject);

        vk::CommandPool getNewCommandPool();
        void pushCommandBuffer(vk::CommandBuffer&);

        // Events

        void _onResize();
    private:
        // One-time creation

        bool CreateInstance();      
        bool SetupDM();
        bool CreateSurface();
        bool PickPhysicalDevice();
        bool CreateLogicalDevice();
        bool CreateAllocator();
        
        void DestroyObjects();
        void DestroyShaders();
        void DestroyAllocator();
        void DestroyLogicalDevice();
        void DestroySurface();
        void DestroyDM();
        void DestroyInstance();

        // On-resize creation

        bool CreateSwapchain();
        bool CreateImageViews();    
        bool CreateRenderPass();    
        bool CreateFramebuffers();  
        bool CreateCommandPool();   
        bool CreateSyncObjects();   
        
        bool RecreateSwapchain();
        
        void DestroySyncObjects();
        void DestroyCommandPool();
        void DestroyImageViews();
        void DestroyRenderPass();
        void DestroyFramebuffers();
        void DestroySwapchain();

        bool AllocateCommandBuffers();
        
        // On-draw creation

        bool CreateCommandBuffer(int);

        // Utils

        VkObject& GetEmptyObjectSlot(PObject*);

        PShader GetDefaultShader();
        bool CreateShader(ShaderCreateInfo&&, PShader*) = delete;
        bool CreateShader(ShaderCreateInfo&, PShader*);
        void DestroyShader(PShader);

        // Variables

        std::unordered_map<PShader, VkShader> m_Shaders = {};
        std::vector<std::pair<bool, VkObject>> m_Objects = {};
        // ^--- the bool represents whether the spot is occupied or not

        std::vector<vk::CommandBuffer> m_SecondaryBuffers = {};
        // ^--- allow external command buffer registeirng
        std::mutex m_SecondaryBuffersMutex = {};
        
        struct {
            char useLayers : 1 = 1;
            char layersUsed : 1;
            char minimized : 1;
            char active : 1;
        } m_States;

        PresentMode m_PresentMode = PresentMode::eAny;
        Window* m_Window = nullptr;
        mut_u16 m_ID = -1;

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

        std::array<const char*, 1> m_Extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        std::array<const char*, 1> m_Layers = {
            "VK_LAYER_KHRONOS_validation"
        };
    };
}
}