#include "utils/debug.hpp"
#include "graphics/window.hpp"
#include "graphics/vk_renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <fmt/core.h>

using namespace std;

namespace Aery { namespace Graphics {
    bool VkRenderer::CreateSurface() {
        VkSurfaceKHR Surface = VK_NULL_HANDLE;
        VkResult Result = glfwCreateWindowSurface(static_cast<VkInstance>(m_Instance), m_Window->info().handle, nullptr, &Surface);  
        if (Result != VK_SUCCESS) {
            Aery::error(fmt::format("<VkRenderer::CreateSurface> ID {} failed to create a surface.", m_ID));
            return false;
        }

        m_Surface = static_cast<vk::SurfaceKHR>(Surface);
        Aery::log(fmt::format("<VkRenderer::CreateSurface> ID {} created a surface.", m_ID), fmt::color::light_green);
        return true;
    }

    void VkRenderer::DestroySurface() {
        vkDestroySurfaceKHR(static_cast<VkInstance>(m_Instance), static_cast<VkSurfaceKHR>(m_Surface), nullptr);
        Aery::log(fmt::format("<VkRenderer::DestroySurface> ID {} destroyed a surface.", m_ID));
    }
}
}