#include "Shared.hpp"
#include "GraphicsSystem.hpp"
#include "GraphicsStats.hpp"
#include "Platform/Window.hpp"

Graphics::System::~System()
{
    LOG("Destroying graphics system...");
}

bool Graphics::System::Setup(Platform::Window* window)
{
    LOG("Creating graphics system...");

    ASSERT(window);
    m_window = window;

    LOG_SUCCESS("Created graphics system");
    return true;
}

void Graphics::System::BeginFrame()
{
}

void Graphics::System::EndFrame()
{
    Stats& stats = Stats::Get();
    stats.OnEndFrame();

    if(stats.HasUpdated())
    {
        auto titleStats =
            InlineString<64>::Format(" - %.2f FPS (%.2f ms)",
            stats.GetFramesPerSecond(), stats.GetFrameTimeAverage() * 1000.0f);
        m_window->SetTitleSuffix(titleStats.GetData());
    }
}
