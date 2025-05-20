#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Platform/Time.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Graphics/System.hpp"
#include "Graphics/Stats.hpp"

int main(const int argc, const char* const* argv)
{
    Engine::Setup({
        .applicationName = "Bourne Engine Example",
        .commandLineArguments = argv,
        .commandLineArgumentCount = argc,
    });

    Platform::Window window;
    if(!window.Setup())
    {
        LOG_FATAL("Failed to setup platform window");
        return -1;
    }

    Graphics::System graphics;
    if(!graphics.Setup(&window))
    {
        LOG_FATAL("Failed to setup graphics system");
        return -1;
    }

    LOG_INFO("Starting main loop...");
    window.Show();

    Time::Timer timer;
    while(true)
    {
        float deltaTime = timer.Tick();

        Platform::Window::ProcessEvents();
        if(window.IsClosing())
            break;

        graphics.BeginFrame();
        {
        }
        graphics.EndFrame();

    #if !CONFIG_RELEASE
        Graphics::Stats& graphicsStats = Graphics::Stats::Get();
        Memory::Stats& memoryStats = Memory::Stats::Get();

        static Time::IntervalTimer titleUpdateTimer(0.2f);
        if(titleUpdateTimer.Tick())
        {
            auto titleStats = InlineString<128>::Format(
                " | %.0f FPS (min: %.2fms, avg: %.2fms, max: %.2fms) | Allocations: %llu (%llu bytes)",
                graphicsStats.GetFramesPerSecond(),
                graphicsStats.GetFrameTimeMinimum() * 1000.0f,
                graphicsStats.GetFrameTimeAverage() * 1000.0f,
                graphicsStats.GetFrameTimeMaximum() * 1000.0f,
                memoryStats.GetAllocatedFrameCount(),
                memoryStats.GetAllocatedFrameBytes());

            window.SetTitleSuffix(titleStats.GetData());
        }

        memoryStats.ResetFrameAllocations();
    #endif
    }

    LOG_INFO("Exiting application...");
    return 0;
}
