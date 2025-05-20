#include "Shared.hpp"
#include "Window.hpp"
#include "Platform/Window.hpp"

class WindowClass
{
public:
    explicit WindowClass(const WNDPROC WndProc)
    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WndProc;
        wc.lpszClassName = GetClassName();
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        ASSERT_EVALUATE(RegisterClassEx(&wc) != 0);
    }

    ~WindowClass()
    {
        ASSERT_EVALUATE(UnregisterClass(GetClassName(), nullptr) != 0);
    }

    static const char* GetClassName()
    {
        return "WindowClass";
    }
};

static Platform::Window* GetWindowInstanceFromUserData(const HWND hwnd)
{
    auto* instance = reinterpret_cast<Platform::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    return instance;
}

LRESULT CALLBACK Platform::Detail::Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Platform::Window* window = GetWindowInstanceFromUserData(hwnd);

    switch(uMsg)
    {
    case WM_CLOSE:
        window->Close();
        break;

    case WM_EXITSIZEMOVE:
        RECT windowRect;
        GetClientRect(hwnd, &windowRect);
        window->OnResizeEvent(windowRect.right, windowRect.bottom);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Platform::Window::ProcessEvents()
{
    MSG msg = {};
    while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool Platform::Window::OnSetup()
{
    ASSERT(!m_detail.m_handle);

    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    RECT windowRect = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
    AdjustWindowRectEx(&windowRect, windowStyle, false, 0);

    static WindowClass windowClass(Detail::Window::WndProc);
    m_detail.m_handle = CreateWindowEx(0, windowClass.GetClassName(), m_title.GetData(),
        windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top, nullptr, nullptr, nullptr, this);

    if(m_detail.m_handle == nullptr)
    {
        LOG_ERROR("Failed to create Win32 window (error code %i)", GetLastError());
        return false;
    }

    SetWindowLongPtr(m_detail.m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    GetClientRect(m_detail.m_handle, &windowRect);
    m_width = windowRect.right;
    m_height = windowRect.bottom;
    UpdateTitle();

    LOG_SUCCESS("Created Win32 window");
    return true;
}

void Platform::Window::OnDestroy()
{
    if(m_detail.m_handle)
    {
        DestroyWindow(m_detail.m_handle);
    }
}

void Platform::Window::OnResize(const u32 width, const u32 height)
{
    ASSERT_SLOW(m_detail.m_handle);
    RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, false, 0);
    SetWindowPos(m_detail.m_handle, nullptr, 0, 0,
        windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
        SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
    UpdateWindow(m_detail.m_handle);

    GetClientRect(m_detail.m_handle, &windowRect);
    OnResizeEvent(windowRect.right, windowRect.bottom);
}

void Platform::Window::OnUpdateTitle(const char* title)
{
    ASSERT_SLOW(title);
    ASSERT_SLOW(m_detail.m_handle);
    if(!SetWindowText(m_detail.m_handle, title))
    {
        LOG_ERROR("Failed to update Win32 window title (error code %i)", GetLastError());
    }
}

void Platform::Window::OnUpdateVisibility()
{
    ASSERT_SLOW(m_detail.m_handle);
    ShowWindow(m_detail.m_handle, m_visible ? SW_SHOW : SW_HIDE);
}
