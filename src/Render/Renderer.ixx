module;
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <array>
export module Lunar:Renderer;
import :Numbers;
import :Window;

export namespace Lunar {

    using RenderObject = i32;
    using RenderShader = i32;

    class Renderer {
    public:
        Renderer() = default;
        ~Renderer() = default;

        bool start(Lunar::Window& Window);
        void stop();

        void draw();
    private:
        void CreatePermanentResources();
        void DestroyPermanentResources();

        /* Those resources are: */

        void PickGPU();
        void SetupMessenger();  void DestroyMessenger();
        void CreateSurface();   void DestroySurface();
        void CreateDevice();    void DestroyDevice();
        void CreateAllocator(); void DestroyAllocator();

        /* Temporary Resources */
        
        void CreateSwapchain();    void DestroySwapchain();
        void CreateImageViews();   void DestroyImageViews();
        void CreateRenderPass();   void DestroyRenderPass();
        void CreateFramebuffers(); void DestroyFramebuffers();
        void CreateCommandPool();  void DestroyCommandPool();
        void AllocateCommandBuffers();

    private:
        std::array<const char*, 1> m_Extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        std::array<const char*, 1> m_Layers = {
            "VK_LAYER_KHRONOS_validation"
        };

        VkDebugUtilsMessengerEXT m_DebugMsg = NULL;
        VkSurfaceKHR m_Surface = NULL;
        VkPhysicalDevice m_PhysDevice = NULL;
        VkDevice m_Device = NULL;
        VkQueue m_GraphicsQ = NULL;
        VkQueue m_PresentQ = NULL;

        VmaAllocator m_Allocator = NULL;

        Window* m_Window;
        u32 m_ID = 0;
    };
}
