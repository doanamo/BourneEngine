#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Engine.hpp"

Platform::Window::~Window()
{
    if(m_setup)
    {
        LOG_DEBUG("Destroying platform window...");
    }
}

bool Platform::Window::Setup(const WindowConfig& config)
{
    ASSERT(!m_setup);
    LOG_DEBUG("Setting up platform window...");

    m_title = "Bourne Engine";
    m_width = config.width;
    m_height = config.height;

    m_detail.SetOnCloseEvent([this]()
    {
        OnCloseEvent();
    });

    if(!m_detail.Setup(m_title, m_width, m_height))
    {
        return false;
    }

    LOG_INFO("Created %ux%u window", m_width, m_height);
    LOG_SUCCESS("Platform window setup complete");
    return m_setup = true;
}

void Platform::Window::ProcessEvents()
{
    m_detail.ProcessEvents();

    u32 width, height;
    m_detail.GetSize(width, height);
    HandleResize(width, height);
}

void Platform::Window::Show()
{
    m_visible = true;
    m_detail.SetVisibility(m_visible);
}

void Platform::Window::Hide()
{
    m_visible = false;
    m_detail.SetVisibility(m_visible);
}

void Platform::Window::Close()
{
    LOG_INFO("Window close requested");
    m_closing = true;
}

void Platform::Window::SetSize(u32 width, u32 height)
{
    m_detail.SetSize(width, height);
}

void Platform::Window::SetTitle(const StringView& title)
{
    m_title = title;
    UpdateTitle();
}

void Platform::Window::SetTitleSuffix(const StringView& suffix)
{
    m_titleSuffix = suffix;
    UpdateTitle();
}

void Platform::Window::UpdateTitle()
{
    InlineString<256> fullTitle;
    fullTitle += m_title;
    fullTitle += m_titleSuffix;
    m_detail.SetTitle(fullTitle.GetData());
}

void Platform::Window::OnCloseEvent()
{
    Close();
}

void Platform::Window::HandleResize(u32 width, u32 height)
{
    if(width == 0 || height == 0)
        return;

    if(width == m_width && height == m_height)
        return;

    m_width = width;
    m_height = height;

    LOG_INFO("Window resized to %ux%u", m_width, m_height);
    m_resizeDelegate.Broadcast(m_width, m_height);
}
