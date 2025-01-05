#include "Shared.hpp"
#include "Platform/Window.hpp"

Platform::Window::~Window()
{
    Close();
}

bool Platform::Window::Open(const StringView& title, const u32 width, const u32 height)
{
    ASSERT(!m_open);

    LOG("Creating window...");

    m_title = title;
    m_width = width;
    m_height = height;

    if(!WindowImpl::Open(*this))
        return false;

    ASSERT(m_open, "Window implementation should have set m_open to true!");
    LOG("Window dimmensions: %ux%u", m_width, m_height);
    return true;
}

void Platform::Window::Close()
{
    if(m_open)
    {
        WindowImpl::Close(*this);
        m_open = false;
        m_private = nullptr;
    }
}

void Platform::Window::ProcessEvents()
{
    WindowImpl::ProcessEvents();
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

const char* Platform::Window::GetVulkanExtension()
{
    return WindowImpl::GetVulkanExtension();
}

void Platform::Window::UpdateTitle()
{
    ASSERT(m_open);
    InlineString<256> fullTitle;
    fullTitle += m_title;
    fullTitle += m_titleSuffix;
    WindowImpl::UpdateTitle(*this, fullTitle.GetData());
}

void Platform::Window::OnCloseEvent()
{
    Close();
}

void Platform::Window::OnResizeEvent(u32 width, u32 height)
{
    if(m_width == width && m_height == height)
        return;

    m_width = width;
    m_height = height;
    LOG("Window resized to %ux%u", m_width, m_height);
}
