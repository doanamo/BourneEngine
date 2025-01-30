#pragma once

#if defined(PLATFORM_WINDOWS)
    #include "Windows/Window.hpp"
#elif defined(PLATFORM_LINUX)
    #include "Linux/Window.hpp"
#else
    #error "Unknown platform!"
#endif

namespace Platform
{
    class Window final : private NonCopyable
    {
        WindowPrivate m_private;
        HeapString m_title;
        HeapString m_titleSuffix;
        u32 m_width = 1024;
        u32 m_height = 576;
        bool m_visible = false;
        bool m_closing = false;

        struct PrivateConstructorTag
        {
            explicit PrivateConstructorTag() = default;
        };

    public:
        static UniquePtr<Window> Create();
        static void ProcessEvents();

        Window(PrivateConstructorTag);
        ~Window();

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

        static const char* GetVulkanSurfaceExtension();

    private:
        void UpdateTitle();

        bool OnCreate();
        void OnDestroy();
        void OnResize(u32 width, u32 height);
        void OnUpdateTitle(const char* title);
        void OnUpdateVisibility();

        void OnCloseEvent();
        void OnResizeEvent(u32 width, u32 height);
    };
}
