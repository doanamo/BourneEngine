#pragma once

namespace Platform
{
    class Window;
}

namespace Platform::Detail
{
    class Window
    {
        friend Platform::Window;
        HWND m_handle = nullptr;
        bool m_resizing = false;

    public:
        Window() = default;
        ~Window() = default;

        HWND GetHandle() const
        {
            return m_handle;
        }

    private:
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}
