#include "Shared.hpp"
#include "Platform/Window.hpp"

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch(uMsg)
    {
    case WM_NCCREATE:
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);
        break;

    case WM_DESTROY:
        window->m_hwnd = nullptr;
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::~Window()
{
    if(m_hwnd)
    {
        DestroyWindow(m_hwnd);
    }
}

bool Window::Open(const char* title, u32 width, u32 height)
{
    ASSERT(m_hwnd == nullptr);

    static bool ClassRegistered = false;
    const char ClassName[] = "Window";

    if(!ClassRegistered)
    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = Window::WndProc;
        wc.lpszClassName = &ClassName[0];
        if(!RegisterClassEx(&wc))
        {
            LOG_ERROR("Failed to register window class");
            return false;
        }

        ClassRegistered = true;
    }

    m_hwnd = CreateWindowEx(0, &ClassName[0], title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, nullptr, this);

    if(m_hwnd == nullptr)
    {
        LOG_ERROR("Failed to create window");
        return false;
    }

    ShowWindow(m_hwnd, SW_NORMAL);
    UpdateWindow(m_hwnd);

    LOG_INFO("Created window");
    return true;
}

void Window::ProcessEvents()
{
    MSG msg = {};
    while(PeekMessageW(&msg, m_hwnd, 0, 0, PM_REMOVE) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::Close()
{
    DestroyWindow(m_hwnd);
}

bool Window::IsOpen() const
{
    return m_hwnd;
}
