#include "Shared.hpp"
#include "Window.hpp"

void Platform::Detail::Window::ProcessEvents()
{
}

void Platform::Detail::Window::SetOnCloseEvent(OnWindowCloseFunction&& function)
{
}

void Platform::Detail::Window::SetOnResizeEvent(OnWindowResizeFunction&& function)
{
}

bool Platform::Detail::Window::Setup(const StringView& title, u32& width, u32& height)
{
    return true;
}

void Platform::Detail::Window::Resize(u32 width, u32 height)
{
}

void Platform::Detail::Window::UpdateTitle(const StringView& title)
{
}

void Platform::Detail::Window::UpdateVisibility(bool visible)
{
}
