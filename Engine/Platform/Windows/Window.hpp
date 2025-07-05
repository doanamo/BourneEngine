#pragma once

namespace Platform
{
    class Window;
}

namespace Platform::Detail
{
    class Window
    {
        HWND m_handle = nullptr;
        bool m_resizing = false;

        OnWindowCloseFunction m_onCloseFunction;
        OnWindowResizeFunction m_onResizeFunction;

    public:
        static void ProcessEvents();

        Window() = default;
        ~Window();

        void SetOnCloseEvent(OnWindowCloseFunction&& function);
        void SetOnResizeEvent(OnWindowResizeFunction&& function);
        bool Setup(const StringView& title, u32& width, u32& height);

        void Resize(u32 width, u32 height);
        void UpdateTitle(const StringView& title);
        void UpdateVisibility(bool visible);

        HWND GetHandle() const
        {
            return m_handle;
        }

    private:
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}
