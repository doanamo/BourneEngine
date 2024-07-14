#pragma once

namespace Platform
{
    class Window final
    {
    private:
        u32 m_width = 0;
        u32 m_height = 0;

    public:
        Window() = default;
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool Open(const char* title, u32 width, u32 height);
        void ProcessEvents();
        void Close();

        bool IsOpen() const;

        u32 GetWidth() const
        {
            return m_width;
        }

        u32 GetHeight() const
        {
            return m_height;
        }

#ifdef PLATFORM_WINDOWS
        HWND GetHandle() const
        {
            return m_hwnd;
        }
#endif

    private:
#ifdef PLATFORM_WINDOWS
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        HWND m_hwnd = nullptr;
#endif
    };
}
