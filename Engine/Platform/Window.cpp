#include "Shared.hpp"
#include "Platform/Window.hpp"

Platform::Window::~Window()
{
    Close();
}

Platform::Window::OpenResult Platform::Window::Open(const char* title, const u32 width, const u32 height)
{
    ASSERT(!m_open);

    LOG("Creating window...");

    m_title = title;
    m_width = width;
    m_height = height;

    const OpenResult result = OnOpen();
    if(result.IsFailure())
    {
        return result;
    }

    m_open = true;
    LOG("Window dimmensions: %ux%u", m_width, m_height);
    return result;
}

void Platform::Window::Close()
{
    if(m_open)
    {
        OnClose();
        m_open = false;
        m_private = nullptr;
    }
}

void Platform::Window::ProcessEvents()
{
    ASSERT(m_open);
    OnProcessEvents();
}

void Platform::Window::SetTitle(const char* title)
{
    ASSERT(m_open);
    m_title = title;
    OnSetTitle();
}
