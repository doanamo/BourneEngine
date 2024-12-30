#pragma once

namespace Platform
{
    class Window final
    {
        HeapString m_title;
        HeapString m_titleSuffix;
        u32 m_width = 0;
        u32 m_height = 0;
        bool m_open = false;

        UniquePtr<void> m_private;

    public:
        enum class OpenError
        {
            CreateWindowFailed,
        };

        using OpenResult = Result<void, OpenError>;

    public:
        Window() = default;
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        OpenResult Open(const StringView& title, u32 width, u32 height);
        void ProcessEvents();
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

    private:
        void UpdateTitle();

        OpenResult OnOpen();
        void OnClose();
        void OnProcessEvents();
        void OnUpdateTitle(const char* title);
    };
}
