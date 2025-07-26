#include "Shared.hpp"
#include "Window.hpp"

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

static Platform::Detail::Window* GetWindowInstanceFromUserData(const HWND hwnd)
{
    auto* instance = reinterpret_cast<Platform::Detail::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    return instance;
}

LRESULT CALLBACK Platform::Detail::Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* window = GetWindowInstanceFromUserData(hwnd);

    switch(uMsg)
    {
    case WM_CLOSE:
        window->m_onCloseFunction();
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Platform::Detail::Window::~Window()
{
    if(m_handle)
    {
        DestroyWindow(m_handle);
    }
}

void Platform::Detail::Window::SetOnCloseEvent(OnWindowCloseFunction&& function)
{
    m_onCloseFunction = Forward<OnWindowCloseFunction>(function);
}

bool Platform::Detail::Window::Setup(const StringView& title, u32& width, u32& height)
{
    ASSERT(!m_handle);
    ASSERT(m_onCloseFunction.IsBound());

    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    DWORD windowStyleEx = WS_EX_OVERLAPPEDWINDOW;
    RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRectEx(&windowRect, windowStyle, false, windowStyleEx);

    static WindowClass windowClass(Detail::Window::WndProc);
    m_handle = CreateWindowEx(windowStyleEx, windowClass.GetClassName(), title.GetData(),
        windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top, nullptr, nullptr, nullptr, this);

    if(m_handle == nullptr)
    {
        LOG_ERROR("Failed to create Win32 window (error code %i)", GetLastError());
        return false;
    }

    SetWindowLongPtr(m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    GetClientRect(m_handle, &windowRect);
    width = windowRect.right;
    height = windowRect.bottom;
    SetTitle(title);

    LOG_SUCCESS("Created Win32 window");
    return true;
}

void Platform::Detail::Window::ProcessEvents()
{
    MSG message = {};
    while(PeekMessage(&message, m_handle, 0, 0, PM_REMOVE) != 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

void Platform::Detail::Window::SetSize(u32 width, u32 height)
{
    ASSERT_SLOW(m_handle);
    RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, false, 0);
    SetWindowPos(m_handle, nullptr, 0, 0,
        windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
        SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
    UpdateWindow(m_handle);
}

void Platform::Detail::Window::SetTitle(const StringView& title)
{
    ASSERT_SLOW(m_handle);
    if(!SetWindowText(m_handle, title.GetData()))
    {
        LOG_ERROR("Failed to update Win32 window title (error code %i)", GetLastError());
    }
}

void Platform::Detail::Window::SetVisibility(bool visible)
{
    ASSERT_SLOW(m_handle);
    ShowWindow(m_handle, visible ? SW_SHOW : SW_HIDE);
}

void Platform::Detail::Window::GetSize(u32 &width, u32 &height)
{
    ASSERT_SLOW(m_handle);

    RECT windowRect;
    GetClientRect(m_handle, &windowRect);

    width = windowRect.right;
    height = windowRect.bottom;
}
