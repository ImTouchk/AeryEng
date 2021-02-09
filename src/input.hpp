#pragma once

#include "utils/types.hpp"
#include <unordered_map>

struct GLFWwindow;

namespace Aery {
    namespace Graphics {
        class Window;
    }

    enum class Key {
        eA = GLFW_KEY_A, eB = GLFW_KEY_B, eC = GLFW_KEY_C, eD = GLFW_KEY_D,
        eE = GLFW_KEY_E, eF = GLFW_KEY_F, eG = GLFW_KEY_G, eH = GLFW_KEY_H,
        eI = GLFW_KEY_I, eJ = GLFW_KEY_J, eK = GLFW_KEY_K, eL = GLFW_KEY_L,
        eM = GLFW_KEY_M, eN = GLFW_KEY_N, eO = GLFW_KEY_O, eP = GLFW_KEY_P,
        eQ = GLFW_KEY_Q, eR = GLFW_KEY_R, eS = GLFW_KEY_S, eT = GLFW_KEY_T,
        eU = GLFW_KEY_U, eV = GLFW_KEY_V, eW = GLFW_KEY_W, eX = GLFW_KEY_X,
        eY = GLFW_KEY_Y, eZ = GLFW_KEY_Z, e0 = GLFW_KEY_0, e1 = GLFW_KEY_1,
        e2 = GLFW_KEY_2, e3 = GLFW_KEY_3, e4 = GLFW_KEY_4, e5 = GLFW_KEY_5,
        e6 = GLFW_KEY_6, e7 = GLFW_KEY_7, e8 = GLFW_KEY_8, e9 = GLFW_KEY_9,
        eF1 = GLFW_KEY_F1, eF2 = GLFW_KEY_F2, eF3 = GLFW_KEY_F3,
        eF4 = GLFW_KEY_F4, eF5 = GLFW_KEY_F5, eF6 = GLFW_KEY_F6,
        eF7 = GLFW_KEY_F7, eF8 = GLFW_KEY_F8, eF9 = GLFW_KEY_F9,
        eF10 = GLFW_KEY_F10, eF11 = GLFW_KEY_F11, eF12 = GLFW_KEY_F12,
        eSpace  = GLFW_KEY_SPACE,      eTab    = GLFW_KEY_TAB, 
        eSlash  = GLFW_KEY_SLASH,      ePageUp = GLFW_KEY_PAGE_UP, 
        ePageDn = GLFW_KEY_PAGE_DOWN,  eEnd    = GLFW_KEY_END,
        eEnter  = GLFW_KEY_ENTER,      eEqual  = GLFW_KEY_EQUAL, 
        eEscape = GLFW_KEY_ESCAPE,     eInsert = GLFW_KEY_INSERT, 
        eCaps   = GLFW_KEY_CAPS_LOCK,  eComma  = GLFW_KEY_COMMA,
    };

    enum class MouseButton {
        eLeft = GLFW_MOUSE_BUTTON_LEFT, eRight = GLFW_MOUSE_BUTTON_RIGHT,
        eMiddle = GLFW_MOUSE_BUTTON_MIDDLE,
        e1 = GLFW_MOUSE_BUTTON_1, e2 = GLFW_MOUSE_BUTTON_2,
        e3 = GLFW_MOUSE_BUTTON_3, e4 = GLFW_MOUSE_BUTTON_4,
        e5 = GLFW_MOUSE_BUTTON_5, e6 = GLFW_MOUSE_BUTTON_6,
        e7 = GLFW_MOUSE_BUTTON_7, e8 = GLFW_MOUSE_BUTTON_8,
    };

    class Input {
    public:
        Input(); ~Input();


        bool isKey(Key);
        bool isButton(MouseButton);

        void _onWindowCreated(Graphics::Window&);
        GLFWwindow* _getHandle();

        void _setKey(Key, int);
        void _setButton(MouseButton, int);
        void _setCursorPos(mut_f32, mut_f32);

    private:
        std::unordered_map<Key, int> m_Keys;
        std::unordered_map<MouseButton, int> m_MouseBtns;
        std::pair<mut_f32, mut_f32> m_Cursor;
        Graphics::Window* m_Window = nullptr;
        GLFWwindow* m_Handle = nullptr;
        mut_u16 m_ID = 0;
    };
}
