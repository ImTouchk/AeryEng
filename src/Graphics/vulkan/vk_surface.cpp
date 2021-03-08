#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "Types.h"
#include "Debug.h"
#include "Graphics/VkCommon.h"
#include "Graphics/Renderer.h"

namespace Lunar {
    void Renderer::CreateSurface()
    {
        VkSurfaceKHR Surface = NULL;
        VkResult Result;
        Result = glfwCreateWindowSurface(
            vk::getInstance(),
            m_Window->m_Handle,
            NULL,
            &Surface
        );
        if (Result != VK_SUCCESS) {
            Lunar::Error("Renderer> Failed to create a surface.");
            Lunar::Exit();
        }
        else Lunar::Print("Renderer> Surface created.");

        m_Surface = Surface;
    }

    void Renderer::DestroySurface()
    {
        if (m_Surface == NULL) {
            return;
        }

        vkDestroySurfaceKHR(vk::getInstance(), m_Surface, NULL);
        Lunar::Print("Renderer> Surface destroyed.");
    }
}
