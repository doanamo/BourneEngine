#include "Shared.hpp"
#include "System.hpp"
#include "Stats.hpp"
#include "Platform/Window.hpp"

Graphics::System::System()
{
    LOG_INFO("Creating graphics system...");
}

Graphics::System::~System()
{
    LOG_INFO("Destroying graphics system...");
}

bool Graphics::System::Setup(Platform::Window* window)
{
    ASSERT(window);
    m_window = window;

    if(!OnSetup())
    {
        LOG_ERROR("Failed to initialize graphics system");
        return false;
    }

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

    //if(stats.HasUpdated()) // #todo: Implement periodic updates
    {
        auto titleStats = InlineString<64>::Format(
            " - %.2fFPS (avg: %.2fms, min: %.2fms, max: %.2fms)",
            stats.GetFramesPerSecond(),
            stats.GetFrameTimeAverage() * 1000.0f,
            stats.GetFrameTimeMinimum() * 1000.0f,
            stats.GetFrameTimeMaximum() * 1000.0f);

        m_window->SetTitleSuffix(titleStats.GetData());
    }
}
