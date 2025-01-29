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
        u32 m_width = 0;
        u32 m_height = 0;
        bool m_open = false;

    public:
        Window() = default;
        ~Window();

        static void ProcessEvents();

        bool Open();
        void Close();

        void Resize(u32 width, u32 height);
        void SetTitle(const StringView& title);
        void SetTitleSuffix(const StringView& suffix);

        bool IsOpen() const
        {
            return m_open;
        }

        u32 GetWidth() const
        {
            return m_width;
        }

        u32 GetHeight() const
        {
            return m_height;
        }

        static const char* GetVulkanSurfaceExtension();

    private:
        void UpdateTitle();

        bool OnOpen();
        void OnClose();
        void OnResize(u32 width, u32 height);
        void OnUpdateTitle(const char* title);

        void OnCloseEvent();
        void OnResizeEvent(u32 width, u32 height);
    };
}
