#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Includes.hpp"

// Note: X11 headers pollute global namespace and conflicts with other source files.
// This source is also excluded from unity build to avoid these includes from leaking.
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#undef Success

Platform::Window::OpenResult Platform::Window::OnOpen()
{
    return OpenResult::Success();
}

void Platform::Window::OnClose()
{
}

void Platform::Window::OnProcessEvents()
{
}

void Platform::Window::OnSetTitle()
{
}
