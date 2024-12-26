#include "Shared.hpp"
#include "Platform/Window.hpp"

Platform::Window::~Window()
{
    Close();
}

Platform::Window::OpenResult Platform::Window::Open(const char* title, const u32 width, const u32 height)
{
    ASSERT(!m_open);
    m_title = title;
    m_width = width;
    m_height = height;

    OpenResult result = OnOpen();
    if(result.IsFailure())
    {
        return result;
    }

    LOG_INFO("Created %ux%u window", m_width, m_height);
    return result;
}

void Platform::Window::Close()
{
    if(m_open)
    {
        OnClose();
    }
}

void Platform::Window::ProcessEvents()
{
    OnProcessEvents();
}

void Platform::Window::SetTitle(const char* title)
{
    m_title = title;
    OnSetTitle();
}
