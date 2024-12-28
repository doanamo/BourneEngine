#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Includes.hpp"

struct WindowPrivate
{
    HWND hwnd = nullptr;
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
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Platform::Window::OpenResult Platform::Window::OnOpen()
{
    ASSERT(!m_open);
    ASSERT(!m_private);

    WindowPrivate* windowPrivate = Memory::New<WindowPrivate>();
    m_private.Reset(windowPrivate,
        [](void* pointer)
        {
            Memory::Delete(static_cast<WindowPrivate*>(pointer));
        });

    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    RECT windowRect = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
    AdjustWindowRect(&windowRect, windowStyle, false);

    static WindowClass windowClass(WndProc);
    windowPrivate->hwnd = CreateWindowEx(0, windowClass.GetClassName(), m_title.GetData(),
        windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top, nullptr, nullptr, nullptr,
        windowPrivate);

    if(windowPrivate->hwnd == nullptr)
    {
        LOG_ERROR("Failed to create window (error code %d)", GetLastError());
        return OpenResult::Failure(OpenError::CreateWindowFailed);
    }

    ShowWindow(windowPrivate->hwnd, SW_NORMAL);
    UpdateWindow(windowPrivate->hwnd);

    GetClientRect(windowPrivate->hwnd, &windowRect);
    m_width = windowRect.right;
    m_height = windowRect.bottom;
    m_open = true;

    return OpenResult::Success();
}

void Platform::Window::OnClose()
{
    ASSERT(m_open);
    ASSERT(m_private);

    WindowPrivate* windowPrivate = static_cast<WindowPrivate*>(m_private.Get());
    DestroyWindow(windowPrivate->hwnd);

    m_open = false;
    m_private = nullptr;
}

void Platform::Window::OnProcessEvents()
{
    ASSERT(m_open);
    ASSERT(m_private);
    WindowPrivate* windowPrivate = static_cast<WindowPrivate*>(m_private.Get());

    MSG msg = {};
    while(PeekMessageW(&msg, windowPrivate->hwnd, 0, 0, PM_REMOVE) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Platform::Window::OnSetTitle()
{
    ASSERT(m_open);
    ASSERT(m_private);
    WindowPrivate* windowPrivate = static_cast<WindowPrivate*>(m_private.Get());
    SetWindowText(windowPrivate->hwnd, m_title.GetData());
}
