#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Includes.hpp"

struct WindowPrivate
{
    HWND handle = nullptr;
    Platform::Window* self = nullptr;
};

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

static WindowPrivate* GetWindowPrivateFromUserData(HWND hwnd)
{
    auto* windowPrivate = reinterpret_cast<WindowPrivate*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    ASSERT(windowPrivate != nullptr);
    return windowPrivate;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_CREATE:
        {
            // Forward extra parameter from CreateWindow() and store it as user data.
            LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));

            WindowPrivate* windowPrivate = GetWindowPrivateFromUserData(hwnd);

            ASSERT(windowPrivate->handle == nullptr);
            windowPrivate->handle = hwnd;

            ASSERT(!windowPrivate->self->m_open);
            windowPrivate->self->m_open = true;
        }
        break;

    case WM_DESTROY:
        {
            WindowPrivate* windowPrivate = GetWindowPrivateFromUserData(hwnd);

            ASSERT(windowPrivate->self->m_open);
            windowPrivate->self->m_open = false;

            ASSERT(windowPrivate->handle == hwnd);
            windowPrivate->handle = nullptr;
        }
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Platform::Window::OpenResult Platform::Window::OnOpen()
{
    WindowPrivate* windowPrivate = Memory::New<WindowPrivate>();
    windowPrivate->self = this;

    ASSERT(m_private == nullptr);
    m_private.Reset(windowPrivate,
        [](void* pointer)
        {
            Memory::Delete(static_cast<WindowPrivate*>(pointer));
        });

    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    RECT windowRect = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
    AdjustWindowRect(&windowRect, windowStyle, false);

    static WindowClass windowClass(WndProc);
    CreateWindowEx(0, windowClass.GetClassName(), m_title.GetData(), windowStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top, nullptr, nullptr, nullptr,
        windowPrivate);

    if(windowPrivate->handle == nullptr)
    {
        LOG_ERROR("Failed to create window (error code %d)", GetLastError());
        return OpenResult::Failure(OpenError::CreateWindowFailed);
    }

    ShowWindow(windowPrivate->handle, SW_NORMAL);
    UpdateWindow(windowPrivate->handle);

    GetClientRect(windowPrivate->handle, &windowRect);
    m_width = windowRect.right;
    m_height = windowRect.bottom;

    return OpenResult::Success();
}

void Platform::Window::OnClose()
{
    ASSERT(m_private != nullptr);
    WindowPrivate* windowPrivate = static_cast<WindowPrivate*>(m_private.get());
    DestroyWindow(windowPrivate->handle);
}

void Platform::Window::OnProcessEvents()
{
    ASSERT(m_private != nullptr);
    WindowPrivate* windowPrivate = static_cast<WindowPrivate*>(m_private.get());

    MSG msg = {};
    while(PeekMessageW(&msg, windowPrivate->handle, 0, 0, PM_REMOVE) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Platform::Window::OnSetTitle()
{
    ASSERT(m_private != nullptr);
    WindowPrivate* windowPrivate = static_cast<WindowPrivate*>(m_private.get());
    SetWindowText(windowPrivate->handle, m_title.GetData());
}
