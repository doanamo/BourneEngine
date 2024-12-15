#include "Shared.hpp"
#include "Platform/Window.hpp"

Platform::Window::~Window()
{
    Close();
}

bool Platform::Window::Open(const char* title, u32 width, u32 height)
{
    ASSERT(!m_open);
    m_title = title;
    m_width = width;
    m_height = height;

    if(!OnCreateWindow())
        return false;

    LOG_INFO("Created %ux%u window", m_width, m_height);
    return true;
}

void Platform::Window::Close()
{
    if(m_open)
    {
        OnDestroyWindow();
    }
}

void Platform::Window::ProcessEvents()
{
    OnProcessEvents();
}

void Platform::Window::SetTitle(const char* title)
{
    m_title = title;
    OnUpdateTitle();
}
