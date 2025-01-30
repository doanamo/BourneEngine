#include "Shared.hpp"
#include "GraphicsSystem.hpp"
#include "GraphicsStats.hpp"
#include "Platform/Window.hpp"
#include "Engine.hpp"

UniquePtr<Graphics::System> Graphics::System::Create(Platform::Window* window)
{
    UniquePtr<System> instance = Memory::New<System>(PrivateConstructorTag{});

    ASSERT(window);
    instance->m_window = window;

    if(!instance->OnCreate())
    {
        LOG_ERROR("Failed to create graphics system");
        return nullptr;
    }

    LOG_SUCCESS("Created graphics system");
    return instance;
}

Graphics::System::System(PrivateConstructorTag)
{
    LOG_INFO("Creating graphics system...");
}

Graphics::System::~System()
{
    LOG_INFO("Destroying graphics system...");
    OnDestroy();
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
