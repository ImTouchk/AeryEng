#include "gl_renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/common.h"
#include "graphics/window.h"
#include "graphics/renderer.h"

namespace
{
    constexpr isize ERROR_GLAD_FAIL = 200;
}

namespace Lunar
{
    GLRenderer::GLRenderer(void* window)
        : m_Active(false),
          m_Window(window)
    {
    }

    bool GLRenderer::start()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Lunar::error("Failed to load the OpenGL driver.");
            exit(ERROR_GLAD_FAIL);
        }

        Lunar::print("OpenGL renderer started.");
        m_Active = true;
        return true;
    }

    void GLRenderer::stop()
    {
        m_Active = false;
    }

    bool GLRenderer::active() const
    {
        return m_Active;
    }
}
