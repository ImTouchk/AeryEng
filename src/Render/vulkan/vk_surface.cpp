module;
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "vk_common.hpp"
module Lunar:Renderer;
import :Window;
import :Debug;

namespace Lunar {
    void Renderer::CreateSurface()
    {
        VkSurfaceKHR Surface = NULL;
        VkResult Result;
        Result = glfwCreateWindowSurface(
            vk::getInstance(),
            m_Window->m_Handle,
            nullptr,
            &Surface
        );
        if (Result != VK_SUCCESS) {
            Lunar::Error("<Vulkan> Failed to create a surface.");
            Lunar::Exit();
        }
        else Lunar::Print("<Vulkan> Surface created.");

        m_Surface = Surface;
    }

    void Renderer::DestroySurface()
    {
        if (m_Surface == NULL) {
            return;
        }

        vkDestroySurfaceKHR(vk::getInstance(), m_Surface, nullptr);
        Lunar::Print("<Vulkan> Surface destroyed.");
    }
}