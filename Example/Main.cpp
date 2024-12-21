#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Platform/Timer.hpp"
#include "Engine/Graphics/GraphicsSystem.hpp"
#include "Engine/Graphics/GraphicsStats.hpp"

int main(const int argc, const char* const* argv)
{
    Engine::Setup(argc, argv);

    Platform::Window window;
    constexpr char windowTitle[] = "Bourne Engine Example";
    if(!window.Open(windowTitle, 1024, 576))
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

    Platform::Timer timer;
    while(true)
    {
        float deltaTime = timer.Tick();

        window.ProcessEvents();
        if(!window.IsOpen())
            break;

        graphics.BeginFrame();
        {
        }
        graphics.EndFrame();

        const Graphics::Stats& graphicsStats = graphics.GetStats();
        if(graphicsStats.HasUpdated())
        {
            auto title = InlineString<64>::Format("%s - %.2f FPS (%.2f ms)",
                windowTitle, graphicsStats.GetFramesPerSecond(),
                graphicsStats.GetFrameTimeAverage() * 1000.0f);
            window.SetTitle(title.GetData());
        }
    }

    LOG_INFO("Exiting application...");
    return 0;
}
