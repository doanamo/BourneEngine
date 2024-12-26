#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Includes.hpp"

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
