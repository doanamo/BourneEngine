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

void Platform::WindowImpl::ProcessEvents()
{
    MSG msg = {};
    while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool Platform::WindowImpl::CreateWindow(Window& self)
{
    ASSERT_SLOW(!self.m_open);
    ASSERT_SLOW(self.m_private);

    auto* windowPrivate = Memory::New<WindowPrivate>();
    self.m_private.Reset(windowPrivate,
        [](void* pointer)
        {
            Memory::Delete(static_cast<WindowPrivate*>(pointer));
        });

    SCOPE_GUARD([&self]()
    {
        if(!self.m_open)
        {
            self.m_private = nullptr;
        }
    });

    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    RECT windowRect = { 0, 0, static_cast<LONG>(self.m_width), static_cast<LONG>(self.m_height) };
    AdjustWindowRect(&windowRect, windowStyle, false);

    static WindowClass windowClass(WndProc);
    windowPrivate->hwnd = CreateWindowEx(0, windowClass.GetClassName(), self.m_title.GetData(),
        windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top, nullptr, nullptr, nullptr,
        windowPrivate);

    if(windowPrivate->hwnd == nullptr)
    {
        LOG_ERROR("Failed to create Win32 window (error code %i)", GetLastError());
        return false;
    }

    ShowWindow(windowPrivate->hwnd, SW_NORMAL);
    UpdateWindow(windowPrivate->hwnd);

    GetClientRect(windowPrivate->hwnd, &windowRect);
    self.m_width = windowRect.right;
    self.m_height = windowRect.bottom;

    self.m_open = true;
    LOG_SUCCESS("Created Win32 window");
    return true;
}

void Platform::WindowImpl::DestroyWindow(Window& self)
{
    ASSERT(!self.m_private);
    auto* windowPrivate = static_cast<WindowPrivate*>(self.m_private.Get());
    ASSERT_SLOW(windowPrivate->hwnd);

    ::DestroyWindow(windowPrivate->hwnd);
}

void Platform::WindowImpl::Resize(Window& self, const u32 width, const u32 height)
{
    ASSERT(!self.m_private);
    auto* windowPrivate = static_cast<WindowPrivate*>(self.m_private.Get());
    ASSERT_SLOW(windowPrivate->hwnd);

    SetWindowPos(windowPrivate->hwnd, 0, 0, 0, width, height, SWP_NOMOVE);
}

bool Platform::WindowImpl::UpdateTitle(Window& self, const char* title)
{
    ASSERT(!self.m_private);
    auto* windowPrivate = static_cast<WindowPrivate*>(self.m_private.Get());
    ASSERT_SLOW(windowPrivate->hwnd);

    ASSERT_SLOW(title);
    if(!SetWindowText(windowPrivate->hwnd, title))
    {
        LOG_ERROR("Failed to set Win32 window title (error code %i)", GetLastError());
        return false;
    }

    return true;
}

const char* Platform::WindowImpl::GetVulkanExtension()
{
    return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
}
