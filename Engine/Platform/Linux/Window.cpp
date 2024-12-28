#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Includes.hpp"

// Note: X11 headers pollute global namespace and conflicts with other source files.
// This source is also excluded from unity build to avoid these includes from leaking.
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#undef Success

static constexpr auto X11WindowEventMask = ExposureMask | ButtonPressMask | KeyPressMask | StructureNotifyMask;

struct WindowPrivate
{
    Display* display = nullptr;
    Window window = 0;

    Atom wmDeleteWindow;
};

int ErrorHandler(Display* display, XErrorEvent* error)
{
    LOG_ERROR("X11 error code: %d", error->error_code);
    return 0;
}

Platform::Window::OpenResult Platform::Window::OnOpen()
{
    ASSERT(!m_private);
    auto* windowPrivate = Memory::New<WindowPrivate>();
    m_private.Reset(windowPrivate,
        [](void* pointer)
        {
            Memory::Delete(static_cast<WindowPrivate*>(pointer));
        });

    XSetErrorHandler(ErrorHandler);

    windowPrivate->display = XOpenDisplay(nullptr);
    windowPrivate->window = XCreateSimpleWindow(windowPrivate->display, DefaultRootWindow(windowPrivate->display),
        0, 0, m_width, m_height, 0, BlackPixel(windowPrivate->display, 0), BlackPixel(windowPrivate->display, 0));

    if(windowPrivate->window == 0)
    {
        LOG_ERROR("Failed to create X11 window");
        return OpenResult::Failure(OpenError::CreateWindowFailed);
    }

    XSetStandardProperties(windowPrivate->display, windowPrivate->window,
        m_title.GetData(), m_title.GetData(), None, nullptr, 0, nullptr);

    XSelectInput(windowPrivate->display, windowPrivate->window, X11WindowEventMask);
    windowPrivate->wmDeleteWindow = XInternAtom(windowPrivate->display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(windowPrivate->display, windowPrivate->window, &windowPrivate->wmDeleteWindow, 1);

    XClearWindow(windowPrivate->display, windowPrivate->window);
    XMapRaised(windowPrivate->display, windowPrivate->window);

    XWindowAttributes attributes;
    XGetWindowAttributes(windowPrivate->display, windowPrivate->window, &attributes);
    m_width = attributes.width;
    m_height = attributes.height;

    LOG_SUCCESS("Created X11 window");
    return OpenResult::Success();
}

void Platform::Window::OnClose()
{
    ASSERT(m_private);
    const auto* windowPrivate = static_cast<WindowPrivate*>(m_private.Get());
    ASSERT_SLOW(windowPrivate->display);
    ASSERT_SLOW(windowPrivate->window);

    XDestroyWindow(windowPrivate->display, windowPrivate->window);
    XCloseDisplay(windowPrivate->display);
}

void Platform::Window::OnProcessEvents()
{
    ASSERT(m_private);
    const auto* windowPrivate = static_cast<WindowPrivate*>(m_private.Get());
    ASSERT_SLOW(windowPrivate->display);
    ASSERT_SLOW(windowPrivate->window);

    XEvent event;
    while(XCheckTypedWindowEvent(windowPrivate->display, windowPrivate->window, ClientMessage, &event))
    {
        if(event.type == ClientMessage && event.xclient.data.l[0] == windowPrivate->wmDeleteWindow)
        {
            Close();
            return;
        }
    }

    while(XCheckWindowEvent(windowPrivate->display, windowPrivate->window, X11WindowEventMask, &event))
    {
    }
}

void Platform::Window::OnSetTitle()
{
    ASSERT(m_private);
    const auto* windowPrivate = static_cast<WindowPrivate*>(m_private.Get());
    ASSERT_SLOW(windowPrivate->display);
    ASSERT_SLOW(windowPrivate->window);

    XStoreName(windowPrivate->display, windowPrivate->window, m_title.GetData());
}
