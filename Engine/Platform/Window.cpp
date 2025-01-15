#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Engine.hpp"

Platform::Window::~Window()
{
    Close();
}

void Platform::Window::ProcessEvents()
{
    WindowImpl::ProcessEvents();
}

bool Platform::Window::Open()
{
    ASSERT(!m_open);

    m_title = InlineString<64>::Format("%s %s", Engine::GetApplicationName(), BuildVersion::Readable);
    m_width = 1024;
    m_height = 576;

    LOG("Creating window...");
    if(!WindowImpl::CreateWindow(*this))
        return false;

    ASSERT(m_open, "Window platform implementation should have set m_open to true");
    return true;
}

void Platform::Window::Close()
{
    if(m_open)
    {
        WindowImpl::DestroyWindow(*this);
        m_open = false;
        m_private = nullptr;
    }
}

void Platform::Window::Resize(const u32 width, const u32 height)
{
    WindowImpl::Resize(*this, width, height);
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
