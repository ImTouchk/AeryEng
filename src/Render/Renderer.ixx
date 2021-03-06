module;
#include <vulkan/vulkan.h>
export module Lunar:Renderer;
import :Numbers;

export namespace Lunar {
    using RenderObject = i32;
    using RenderShader = i32;

    class Renderer {
    public:
        Renderer() = default;
        ~Renderer() = default;

        bool start();
        void stop();

        void draw();
    private:

    private:
        VkDebugUtilsMessengerEXT m_DebugMsg;
        VkSurfaceKHR m_Surface;
        VkPhysicalDevice m_PhysDevice;
        VkDevice m_Device;
        VkQueue m_GraphicsQ;
        VkQueue m_PresentQ;
    };
}