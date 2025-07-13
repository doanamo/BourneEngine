#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Engine.hpp"

void Platform::Window::ProcessEvents()
{
    Detail::Window::ProcessEvents();
}

Platform::Window::~Window()
{
    if(m_setup)
    {
        LOG_DEBUG("Destroying window...");
    }
}

bool Platform::Window::Setup(const WindowConfig& config)
{
    ASSERT(!m_setup);
    LOG_DEBUG("Setting up window...");

    m_title = "Bourne Engine";
    m_width = config.width;
    m_height = config.height;

    m_detail.SetOnCloseEvent([this]()
    {
        OnCloseEvent();
    });

    m_detail.SetOnResizeEvent([this](const u32 width, const u32 height)
    {
        OnResizeEvent(width, height);
    });

    if(!m_detail.Setup(m_title, m_width, m_height))
    {
        LOG_ERROR("Failed to setup window");
        return false;
    }

    LOG_INFO("Created %ux%u window", m_width, m_height);
    LOG_SUCCESS("Window setup complete");
    return m_setup = true;
}

void Platform::Window::Show()
{
    m_visible = true;
    m_detail.UpdateVisibility(m_visible);
}

void Platform::Window::Hide()
{
    m_visible = false;
    m_detail.UpdateVisibility(m_visible);
}

void Platform::Window::Close()
{
    LOG_INFO("Window close requested");
    m_closing = true;
}

void Platform::Window::Resize(const u32 width, const u32 height)
{
    m_detail.Resize(width, height);
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
    m_detail.UpdateTitle(fullTitle.GetData());
}

void Platform::Window::OnCloseEvent()
{
    Close();
}

void Platform::Window::OnResizeEvent(const u32 width, const u32 height)
{
    if(m_width == width && m_height == height)
        return;

    m_width = width;
    m_height = height;
    LOG_INFO("Window resized to %ux%u", m_width, m_height);
}
