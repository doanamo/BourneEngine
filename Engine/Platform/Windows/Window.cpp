#include "Shared.hpp"
#include "Platform/Window.hpp"

class WindowClass
{
public:
    WindowClass(WNDPROC WndProc)
    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WndProc;
        wc.lpszClassName = GetClassName();
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        ASSERT(RegisterClassEx(&wc) != 0);
    }

    ~WindowClass()
    {
        ASSERT(UnregisterClass(GetClassName(), nullptr) != 0);
    }

    const char* GetClassName() const
    {
        return "WindowClass";
    }
};

static Window* GetWindowFromUserData(HWND hwnd)
{
    Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    ASSERT(window);
    return window;
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_NCCREATE:
        // Forward extra parameter from CreateWindow() and store it as user data
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);
        break;

    case WM_DESTROY:
        {
            Window* window = GetWindowFromUserData(hwnd);
            ASSERT(window->m_hwnd == hwnd);
            window->m_hwnd = nullptr;
        }
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

    static WindowClass windowClass(Window::WndProc);
    m_hwnd = CreateWindowEx(0, windowClass.GetClassName(), title, WS_OVERLAPPEDWINDOW,
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
    return m_hwnd != nullptr;
}
