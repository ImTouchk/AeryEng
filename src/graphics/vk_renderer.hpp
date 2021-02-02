#pragma once

#include "utils/types.hpp"
#include <vulkan/vulkan.hpp>
#include <vector>

namespace Aery {
    class Window;
    class VkObject;
    struct VkShader;
    struct VkShaderCreateInfo;
    class VkRenderer {
    public:
        VkRenderer(); ~VkRenderer();

        bool create(Window&&) = delete;
        bool create(Window&);
        void destroy();
        void draw();
        
        // Object methods

        VkObject& getObjectByID(u32);
        bool createDefaultObject(VkObject*, VkShader*);
        bool createObject(VkObject*, VkShader&);
        void destroyObject(VkObject&&) = delete;
        void destroyObject(VkObject&);

        // Shader methods

        VkShader& getShaderByID(u32);
        bool createDefaultShader(VkShader*);
        bool createShader(VkShaderCreateInfo&&, VkShader*) = delete;
        bool createShader(VkShaderCreateInfo&, VkShader*);
        void destroyShader(VkShader&&) = delete;
        void destroyShader(VkShader&);

        void _onResize();
    private:
        // One-time creation

        bool CreateInstance();      void DestroyInstance();
        bool SetupDM();             void DestroyDM();
        bool CreateSurface();       void DestroySurface();
        bool PickPhysicalDevice();
        bool CreateLogicalDevice(); void DestroyLogicalDevice();

        // On-resize creation

        bool RecreateSwapchain();
        bool CreateSwapchain(bool = false);     void DestroySwapchain();
        bool CreateImageViews();    void DestroyImageViews();
        bool CreateRenderPass();    void DestroyRenderPass();
        bool CreateFramebuffers();  void DestroyFramebuffers();
        bool CreateCommandPool();   void DestroyCommandPool();
        bool CreateSyncObjects();   void DestroySyncObjects();

        // On-draw creation

        bool AllocateCommandBuffers();
        bool CreateCommandBuffer(int);

        void DestroyShaders();

        Window* m_Window = nullptr;
        bool m_LayersUsed = false;
        bool m_Minimized = false;
        bool m_UseLayers = true;
        bool m_Active = false;
        u32 m_ID = -1;

        vk::Instance m_Instance;
        vk::DebugUtilsMessengerEXT m_DebugMessenger;
        vk::SurfaceKHR m_Surface;
        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;
        vk::Queue m_GraphicsQ;
        vk::Queue m_PresentQ;
        
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

        std::vector<VkShader> m_Shaders = {};
        std::vector<VkObject> m_Objects = {};
    };
}