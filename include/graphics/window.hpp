#ifndef LUNAR_WINDOW_H
#define LUNAR_WINDOW_H

#include "types.hpp"

namespace Lunar {
    class window {
    public:
        struct createInfo {
            bool vsync      : 1;
            bool resizable  : 1;
            bool maximized  : 1;
            bool fullscreen : 1;
            const char* title;
            usize width, height;
        };

        window(const createInfo&& info);
        ~window();

        bool isActive() const;
        void update();

        isize* handle() const;

    private:
        createInfo m_Info;
        isize* m_Handle;
    };

}

#endif // LUNAR_WINDOW_H
