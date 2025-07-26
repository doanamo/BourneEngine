#include "Shared.hpp"
#include "Window.hpp"

void Platform::Detail::Window::SetOnCloseEvent(OnWindowCloseFunction&& function)
{
}

bool Platform::Detail::Window::Setup(const StringView& title, u32& width, u32& height)
{
    return true;
}

void Platform::Detail::Window::ProcessEvents()
{
}

void Platform::Detail::Window::SetSize(u32 width, u32 height)
{
}

void Platform::Detail::Window::SetTitle(const StringView& title)
{
}

void Platform::Detail::Window::SetVisibility(bool visible)
{
}

void Platform::Detail::Window::GetSize(u32& width, u32& height)
{
}
