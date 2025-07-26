#pragma once

#if defined(PLATFORM_WINDOWS)
    #include "Windows/Window.hpp"
#elif defined(PLATFORM_LINUX)
    #include "Null/Window.hpp"
#else
    #error "Unknown platform"
#endif

namespace Platform
{
    struct WindowConfig;

    class Window final : NonCopyable
    {
        Detail::Window m_detail;
        HeapString m_title;
        HeapString m_titleSuffix;
        u32 m_width = 1024;
        u32 m_height = 576;
        bool m_setup = false;
        bool m_visible = false;
        bool m_closing = false;

        using ResizeDelegate = Delegate<void(u32, u32)>;
        ResizeDelegate m_resizeDelegate;

    public:
        static void ProcessEvents();

        Window() = default;
        ~Window();

        bool Setup(const WindowConfig& config);
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

        ResizeDelegate& GetResizeDelegate()
        {
            return m_resizeDelegate;
        }

        const Detail::Window& GetDetail() const
        {
            return m_detail;
        }

    private:
        void UpdateTitle();
        void OnCloseEvent();
        void OnResizeEvent(u32 width, u32 height);
    };
}
