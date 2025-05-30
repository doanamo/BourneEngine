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

    if(!OnSetup())
    {
        LOG_ERROR("Failed to setup graphics system");
        return false;
    }

    LOG_SUCCESS("Created graphics system");
    return true;
}

void Graphics::System::BeginFrame()
{
    OnBeginFrame();
}

void Graphics::System::EndFrame()
{
    OnEndFrame();

    Stats& stats = Stats::Get();
    stats.OnEndFrame();
}
