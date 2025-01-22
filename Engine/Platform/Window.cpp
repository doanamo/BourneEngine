#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Engine.hpp"

Platform::Window::~Window()
{
    Close();
}

bool Platform::Window::Open()
{
    ASSERT(!m_open);

    m_title = InlineString<64>::Format("%s %s", Engine::GetApplicationName(), EngineVersion::Readable);
    m_width = 1024;
    m_height = 576;

    if(!OnOpen())
    {
        LOG_ERROR("Platform implementation failed to open window");
        return false;
    }

    ASSERT(m_open, "Window platform implementation should have set m_open to true");
    return true;
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

void Platform::Window::Resize(const u32 width, const u32 height)
{
    OnResize(width, height);
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
