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
        glfwMakeContextCurrent(reinterpret_cast<GLFWwindow*>(
            ((Window*)m_Window)->handle()
        ));

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Lunar::error("Failed to load the OpenGL driver.");
            exit(ERROR_GLAD_FAIL);
        }

        glEnable(GL_DEPTH_TEST);

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

    void GLRenderer::clear(fsize r, fsize g, fsize b, fsize a)
    {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(
            static_cast<float>(r), 
            static_cast<float>(g), 
            static_cast<float>(b), 
            static_cast<float>(a)
        );
    }

    void GLRenderer::show()
    {
        glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(
            ((Window*)m_Window)->handle()
        ));
    }
}
