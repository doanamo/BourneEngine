#include "Shared.hpp"
#include "System.hpp"
#include "Stats.hpp"
#include "Platform/Window.hpp"

Graphics::System::~System()
{
    if(m_setup)
    {
        LOG_DEBUG("Destroying graphics system...");
    }
}

bool Graphics::System::Setup(Platform::Window* window)
{
    ASSERT(!m_setup);
    LOG_DEBUG("Setting up graphics system...");

    ASSERT(window);
    m_window = window;

    if(!m_detail.Setup(m_window))
    {
        LOG_ERROR("Failed to setup graphics system");
        return false;
    }

    LOG_SUCCESS("Graphics system setup complete");
    return m_setup = true;
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
