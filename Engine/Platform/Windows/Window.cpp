#include "Shared.hpp"
#include "Platform/Window.hpp"

namespace Platform
{
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
}

static Platform::Window* GetWindowFromUserData(HWND hwnd)
{
    Platform::Window* window = (Platform::Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    ASSERT(window);
    return window;
}

LRESULT CALLBACK Platform::Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CREATE:
        {
            // Forward extra parameter from CreateWindow() and store it as user data
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }
        break;

        case WM_DESTROY:
        {
            Platform::Window* window = GetWindowFromUserData(hwnd);
            ASSERT(window->m_hwnd == hwnd);
            window->m_hwnd = nullptr;
        }
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Platform::Window::~Window()
{
    if(m_hwnd)
    {
        DestroyWindow(m_hwnd);
    }
}

bool Platform::Window::Open(const char* title, u32 width, u32 height)
{
    ASSERT(m_hwnd == nullptr);

    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRect(&windowRect, windowStyle, false);

    static WindowClass windowClass(Window::WndProc);
    m_hwnd = CreateWindowEx(0, windowClass.GetClassName(), title, windowStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top, nullptr, nullptr, nullptr, this);

    if(m_hwnd == nullptr)
    {
        LOG_ERROR("Failed to create window");
        return false;
    }

    ShowWindow(m_hwnd, SW_NORMAL);
    UpdateWindow(m_hwnd);

    LOG_INFO("Created %ux%u window", width, height);
    return true;
}

void Platform::Window::ProcessEvents()
{
    MSG msg = {};
    while(PeekMessageW(&msg, m_hwnd, 0, 0, PM_REMOVE) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Platform::Window::Close()
{
    DestroyWindow(m_hwnd);
}

bool Platform::Window::IsOpen() const
{
    return m_hwnd != nullptr;
}
