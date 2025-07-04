#include "Shared.hpp"
#include "System.hpp"
#include "Stats.hpp"
#include "Platform/Window.hpp"

Graphics::System::System()
{
    LOG_DEBUG("Creating graphics system...");
}

Graphics::System::~System()
{
    LOG_DEBUG("Destroying graphics system...");
}

bool Graphics::System::Setup(Platform::Window* window)
{
    ASSERT(window);
    m_window = window;

    if(!m_detail.Setup(m_window))
    {
        LOG_ERROR("Failed to setup graphics system");
        return false;
    }

    LOG_SUCCESS("Created graphics system");
    return true;
}

void Graphics::System::BeginFrame()
{
    m_detail.BeginFrame(m_window);
}

void Graphics::System::EndFrame()
{
    m_detail.EndFrame();

    Stats& stats = Stats::Get();
    stats.OnEndFrame();
}
