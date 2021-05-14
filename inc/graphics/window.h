#ifndef _LUNAR_GRAPHICS_WINDOW_H
#define _LUNAR_GRAPHICS_WINDOW_H

#include <string_view>
#include "core/common.h"
#include "graphics/renderer.h"

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

        Renderer* getRenderer();

    private:
        void stop();
        void start();

    private:
        friend class GLRenderer;

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
    };
}

#endif // _LUNAR_GRAPHICS_WINDOW_H
