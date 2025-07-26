#pragma once

#include "Platform/WindowEvents.hpp"

namespace Platform
{
    class Window;
}

namespace Platform::Detail
{
    class Window
    {
        HWND m_handle = nullptr;
        OnWindowCloseFunction m_onCloseFunction;

    public:
        Window() = default;
        ~Window();

        void SetOnCloseEvent(OnWindowCloseFunction&& function);
        bool Setup(const StringView& title, u32& width, u32& height);
        void ProcessEvents();

        void SetSize(u32 width, u32 height);
        void SetTitle(const StringView& title);
        void SetVisibility(bool visible);

        void GetSize(u32& width, u32& height);

        HWND GetHandle() const
        {
            return m_handle;
        }

    private:
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}
