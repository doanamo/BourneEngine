#include "Shared.hpp"
#include "Window.hpp"
#include "Platform/Window.hpp"

void Platform::Window::ProcessEvents()
{
}

bool Platform::Window::OnSetup()
{
    LOG_SUCCESS("Created null window");
    return true;
}

void Platform::Window::OnDestroy()
{
}

void Platform::Window::OnResize(const u32 width, const u32 height)
{
}

void Platform::Window::OnUpdateTitle(const char* title)
{
}

void Platform::Window::OnUpdateVisibility()
{
}
