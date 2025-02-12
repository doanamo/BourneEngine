#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Engine.hpp"

Platform::Window::Window()
{
    LOG_INFO("Creating window...");
    m_title = InlineString<64>::Format("%s %s", Engine::GetApplicationName(), EngineVersion::Readable);
}

Platform::Window::~Window()
{
    LOG_INFO("Destroying window...");
    OnDestroy();
}

bool Platform::Window::Setup()
{
    if(!OnSetup())
    {
        LOG_ERROR("Failed to initialize window");
        return false;
    }

    return true;
}

void Platform::Window::Show()
{
    m_visible = true;
    OnUpdateVisibility();
}

void Platform::Window::Hide()
{
    m_visible = false;
    OnUpdateVisibility();
}

void Platform::Window::Close()
{
    LOG_INFO("Window close requested");
    m_closing = true;
}

void Platform::Window::Resize(const u32 width, const u32 height)
{
    OnResize(width, height);
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
    OnUpdateTitle(fullTitle.GetData());
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
