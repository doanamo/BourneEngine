#include "Shared.hpp"
#include "Window.hpp"
#include "Platform/Window.hpp"

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

static Platform::Window* GetWindowInstanceFromUserData(const HWND hwnd)
{
    auto* instance = reinterpret_cast<Platform::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    ASSERT(instance != nullptr);
    return instance;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
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
    ASSERT(!m_detail.hwnd);

    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    RECT windowRect = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
    AdjustWindowRect(&windowRect, windowStyle, false);

    static WindowClass windowClass(WndProc);
    m_detail.hwnd = CreateWindowEx(0, windowClass.GetClassName(), m_title.GetData(),
        windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top, nullptr, nullptr, nullptr, this);

    if(m_detail.hwnd == nullptr)
    {
        LOG_ERROR("Failed to create Win32 window (error code %i)", GetLastError());
        return false;
    }

    GetClientRect(m_detail.hwnd, &windowRect);
    m_width = windowRect.right;
    m_height = windowRect.bottom;

    UpdateTitle();

    LOG_SUCCESS("Created Win32 window");
    return true;
}

void Platform::Window::OnDestroy()
{
    if(m_detail.hwnd)
    {
        DestroyWindow(m_detail.hwnd);
    }
}

void Platform::Window::OnResize(const u32 width, const u32 height)
{
    ASSERT_SLOW(m_detail.hwnd);
    SetWindowPos(m_detail.hwnd, 0, 0, 0, width, height, SWP_NOMOVE);
}

void Platform::Window::OnUpdateTitle(const char* title)
{
    ASSERT_SLOW(title);
    ASSERT_SLOW(m_detail.hwnd);
    if(!SetWindowText(m_detail.hwnd, title))
    {
        LOG_ERROR("Failed to update Win32 window title (error code %i)", GetLastError());
    }
}

void Platform::Window::OnUpdateVisibility()
{
    ASSERT_SLOW(m_detail.hwnd);
    ShowWindow(m_detail.hwnd, m_visible ? SW_SHOW : SW_HIDE);
}

const char* Platform::Window::GetVulkanSurfaceExtension()
{
    return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
}
