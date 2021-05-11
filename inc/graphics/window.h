#ifndef _LUNAR_GRAPHICS_WINDOW_H
#define _LUNAR_GRAPHICS_WINDOW_H

#include <string_view>
#include "core/common.h"

namespace Lunar
{
    class Window
    {
    public:
        struct createInfo
        {
            usize width;
            usize height;
            std::string_view title;
            bool fullscreen;

            static createInfo fromFile(
                const std::string_view path,
                const std::string_view title
            );
        };

        Window(createInfo&& info);
        ~Window();

        bool active() const;
        void update();

    private:
        void stop();
        void start();

    private:
        void*            m_Handle;
        usize            m_Width;
        usize            m_Height;
        std::string_view m_Title;
        bool             m_Fullscreen;
    };
}

#endif // _LUNAR_GRAPHICS_WINDOW_H
