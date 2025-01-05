#pragma once

namespace Platform
{
    class Window final
    {
        friend class WindowImpl;

        ErasedUniquePtr m_private;
        HeapString m_title;
        HeapString m_titleSuffix;
        u32 m_width = 0;
        u32 m_height = 0;
        bool m_open = false;

    public:
        Window() = default;
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        static void ProcessEvents();
        bool Open(const StringView& title, u32 width, u32 height);
        void Close();

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

        static const char* GetVulkanExtension();

    private:
        void UpdateTitle();
        void OnCloseEvent();
        void OnResizeEvent(u32 width, u32 height);
    };

    class WindowImpl
    {
    public:
        static void ProcessEvents();
        static bool Open(Window& self);
        static void Close(Window& self);
        static bool UpdateTitle(Window& self, const char* title);
        static const char* GetVulkanExtension();
    };
}
