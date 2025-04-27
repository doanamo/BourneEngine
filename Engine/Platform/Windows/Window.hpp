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

    public:
        Window() = default;
        ~Window() = default;

        HWND GetHandle() const
        {
            return m_handle;
        }
    };
}
