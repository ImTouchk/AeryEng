#include "utils/debug.hpp"
#include "graphics/window.hpp"
#include "input.hpp"
#include <fmt/core.h>
#include <cassert>
#include <vector>
#include <mutex>

using namespace std;

namespace {
    vector<Aery::Input*> Handlers = {};
    mutex ListMutex = {};

    void KeyCallback(GLFWwindow* Window, int Key_, int Scancode, int Action, int Mods) {
        ListMutex.lock();

        Aery::Input* Current = nullptr;
        for (auto& Handler : Handlers) {
            if (Handler->_getHandle() == Window) {
                Current = Handler;
            }
        }
        assert(Current != nullptr);
        Current->_setKey(static_cast<Aery::Key>(Key_), Action);

        ListMutex.unlock();
    }

    void MouseBtnCallback(GLFWwindow* Window, int Button, int Action, int Mods) {
        ListMutex.lock();

        Aery::Input* Current = nullptr;
        for (auto& Handler : Handlers) {
            if (Handler->_getHandle() == Window) {
                Current = Handler;
            }
        }
        assert(Current != nullptr);
        Current->_setButton(static_cast<Aery::MouseButton>(Button), Action);

        ListMutex.unlock();
    }

    void CursorPosCallback(GLFWwindow* Window, double x, double y) {
        ListMutex.lock();

        Aery::Input* Current = nullptr;
        for (auto& Handler : Handlers) {
            if (Handler->_getHandle() == Window) {
                Current = Handler;
            }
        }
        assert(Current != nullptr);

        Current->_setCursorPos(
            static_cast<Aery::mut_f32>(x), 
            static_cast<Aery::mut_f32>(y)
        );

        ListMutex.unlock();
    }
}

namespace Aery {
    Input::Input() {
        ListMutex.lock();
            Handlers.push_back(this);
            m_ID = Aery::mut_u16( Handlers.size() - 1 );
        ListMutex.unlock();
        m_Keys.reserve(50);
        m_MouseBtns.reserve(10);
    }

    Input::~Input() {
        ListMutex.lock();
            Handlers.erase(
                Handlers.begin() + m_ID
            );
        ListMutex.unlock();
    }

    void Input::_onWindowCreated(Graphics::Window& Handle) {
        m_Window = &Handle;
        m_Handle = m_Window->info().handle;
        glfwSetKeyCallback(m_Handle, (GLFWkeyfun)KeyCallback);
        glfwSetMouseButtonCallback(m_Handle, (GLFWmousebuttonfun)MouseBtnCallback);
        glfwSetCursorPosCallback(m_Handle, (GLFWcursorposfun)CursorPosCallback);

        if (glfwRawMouseMotionSupported()) {
            glfwSetInputMode(m_Handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
    }

    bool Input::isKey(Key Key_) {
        return m_Keys[Key_] == GLFW_PRESS;
    }

    bool Input::isButton(MouseButton Button) {
        return m_MouseBtns[Button] == GLFW_PRESS;
    }

    void Input::_setKey(Key Key_, int State) {
        m_Keys[Key_] = State;
    }

    void Input::_setButton(MouseButton Button, int State) {
        m_MouseBtns[Button] = State;
    }

    void Input::_setCursorPos(mut_f32 x, mut_f32 y) {
        m_Cursor = { x , y };
    }

    GLFWwindow* Input::_getHandle() {
        return m_Window->info().handle;
    }
}