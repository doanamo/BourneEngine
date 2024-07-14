#pragma once

namespace Platform
{
    class Window final
    {
    public:
        Window() = default;
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool Open(const char* title, u32 width, u32 height);
        void ProcessEvents();
        void Close();

        bool IsOpen() const;

    private:
#ifdef PLATFORM_WINDOWS
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        HWND m_hwnd = nullptr;
#endif
    };
}
