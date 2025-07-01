#pragma once

#if defined(PLATFORM_WINDOWS)
    #include "Windows/Window.hpp"
#elif defined(PLATFORM_LINUX)
    #include "Null/Window.hpp"
#else
    #error "Unknown platform define"
#endif

namespace Platform
{
    class Window final : NonCopyable
    {
        // #todo: Replace this detail struct with split implementation into static pimpl (without interface, but type define).
        friend Detail::Window;
        Detail::Window m_detail;
        HeapString m_title;
        HeapString m_titleSuffix;
        u32 m_width = 1024;
        u32 m_height = 576;
        bool m_visible = false;
        bool m_closing = false;

    public:
        static void ProcessEvents();

        Window();
        ~Window();

        bool Setup();
        void Show();
        void Hide();
        void Close();

        void Resize(u32 width, u32 height);
        void SetTitle(const StringView& title);
        void SetTitleSuffix(const StringView& suffix);

        u32 GetWidth() const
        {
            return m_width;
        }

        u32 GetHeight() const
        {
            return m_height;
        }

        bool IsVisible() const
        {
            return m_visible;
        }

        bool IsClosing() const
        {
            return m_closing;
        }

        const Detail::Window& GetDetail() const
        {
            return m_detail;
        }

    private:
        void UpdateTitle();

        bool OnSetup();
        void OnDestroy();
        void OnResize(u32 width, u32 height);
        void OnUpdateTitle(const char* title);
        void OnUpdateVisibility();

        void OnCloseEvent();
        void OnResizeEvent(u32 width, u32 height);
    };
}
