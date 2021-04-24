#ifndef LUNAR_RENDERER_H
#define LUNAR_RENDERER_H

#include <vulkan/vulkan.h>

#include "window.hpp"

namespace Lunar {
    class renderer {
    public:
        struct createInfo {
            window& target;
            bool vsync;
        };

        renderer(createInfo&& info);
        ~renderer();
    
    private:
        void createBoilerplate();
        void destroyBoilerplate();

    private:
        /* VulkanAPI specific */

        VkDebugUtilsMessengerEXT m_DebugMessenger;
        VkSurfaceKHR m_Surface;
        VkPhysicalDevice m_GraphicsCard;
        VkDevice m_Device;
        struct {
            VkQueue present;
            VkQueue graphics;
        } m_Queues;
    };
}

#endif // LUNAR_RENDERER_H
