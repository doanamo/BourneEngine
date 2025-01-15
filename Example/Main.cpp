#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Platform/Time.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Graphics/GraphicsSystem.hpp"

int main(const int argc, const char* const* argv)
{
    Engine::Setup({
        .applicationName = "Bourne Engine Example",
        .commandLineArguments = argv,
        .commandLineArgumentCount = argc,
    });

    Platform::Window window;
    if(!window.Open())
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

    Time::Timer timer;
    while(true)
    {
        float deltaTime = timer.Tick();

        Platform::Window::ProcessEvents();
        if(!window.IsOpen())
            break;

        graphics.BeginFrame();
        {
        }
        graphics.EndFrame();
    }

    LOG_INFO("Exiting application...");
    return 0;
}
