#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Includes.hpp"

namespace
{
    class WindowClass
    {
    public:
        WindowClass(WNDPROC WndProc)
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

        const char* GetClassName() const
        {
            return "WindowClass";
        }
    };

    static Platform::Window* GetWindowFromUserData(HWND hwnd)
    {
        Platform::Window* window = reinterpret_cast<Platform::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        ASSERT(window != nullptr);
        return window;
    }
}

LRESULT CALLBACK Platform::Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_CREATE:
        {
            // Forward extra parameter from CreateWindow() and store it as user data.
            LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));

            Platform::Window* window = GetWindowFromUserData(hwnd);

            ASSERT(window->m_private.handle == nullptr);
            window->m_private.handle = hwnd;

            ASSERT(!window->m_open);
            window->m_open = true;
        }
        break;

    case WM_DESTROY:
        {
            Platform::Window* window = GetWindowFromUserData(hwnd);

            ASSERT(window->m_open);
            window->m_open = false;

            ASSERT(window->m_private.handle == hwnd);
            window->m_private.handle = nullptr;
        }
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Platform::Window::OpenResult Platform::Window::OnOpen()
{
    ASSERT(m_private.handle == nullptr);

    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    RECT windowRect = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
    AdjustWindowRect(&windowRect, windowStyle, false);

    static WindowClass windowClass(Window::WndProc);
    CreateWindowEx(0, windowClass.GetClassName(), m_title.GetData(), windowStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top, nullptr, nullptr, nullptr, this);

    if(m_private.handle == nullptr)
    {
        LOG_ERROR("Failed to create window (error code %d)", GetLastError());
        return OpenResult::Failure(OpenError::CreateWindowFailed);
    }

    ShowWindow(m_private.handle, SW_NORMAL);
    UpdateWindow(m_private.handle);

    GetClientRect(m_private.handle, &windowRect);
    m_width = windowRect.right;
    m_height = windowRect.bottom;

    return OpenResult::Success();
}

void Platform::Window::OnClose()
{
    ASSERT(m_private.handle != nullptr);
    DestroyWindow(m_private.handle);
}

void Platform::Window::OnProcessEvents()
{
    MSG msg = {};
    while(PeekMessageW(&msg, m_private.handle, 0, 0, PM_REMOVE) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Platform::Window::OnSetTitle()
{
    SetWindowText(m_private.handle, m_title.GetData());
}