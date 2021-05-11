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
            bool resizable;
            bool maximized;
            bool  _context;
            usize _samples;
            std::string _renderer;

            static createInfo fromFile(
                const std::string_view path,
                const std::string_view title
            );
        };

        Window(createInfo&& info);
        ~Window();

        bool active() const;
        void update();
        void* handle() const;

    private:
        void stop();
        void start();

    private:
        void*            m_Handle;
        usize            m_Width;
        usize            m_Height;
        usize            m_Samples;
        std::string_view m_Title;
        std::string      m_PreferredRenderer;
        bool             m_Fullscreen;
        bool             m_Resizable;
        bool             m_StartMaximized;
        bool             m_Context;
    };
}

#endif // _LUNAR_GRAPHICS_WINDOW_H
