#pragma once

namespace Platform
{
    class Window final
    {
    private:
        HWND m_hwnd = nullptr;
        u32 m_width = 0;
        u32 m_height = 0;
        String m_title;

    public:
        Window() = default;
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool Open(const char* title, u32 width, u32 height);
        void ProcessEvents();
        void Close();

        bool IsOpen() const;

        void SetTitle(const char* title);

        u32 GetWidth() const
        {
            return m_width;
        }

        u32 GetHeight() const
        {
            return m_height;
        }

        HWND GetHandle() const
        {
            return m_hwnd;
        }

    private:
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}
