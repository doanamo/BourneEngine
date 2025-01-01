#include "Shared.hpp"
#include "Platform/Window.hpp"

Platform::Window::~Window()
{
    Close();
}

Platform::Window::OpenResult Platform::Window::Open(const StringView& title, const u32 width, const u32 height)
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

    ASSERT(m_open, "Window implementation should have set m_open to true!");
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
    OnProcessEvents();
}

void Platform::Window::SetTitle(const StringView& title)
{
    ASSERT(m_open);
    m_title = title;
    UpdateTitle();
}

void Platform::Window::SetTitleSuffix(const StringView& suffix)
{
    ASSERT(m_open);
    m_titleSuffix = suffix;
    UpdateTitle();
}

void Platform::Window::UpdateTitle()
{
    ASSERT(m_open);
    InlineString<256> fullTitle;
    fullTitle += m_title;
    fullTitle += m_titleSuffix;
    OnUpdateTitle(fullTitle.GetData());
}
