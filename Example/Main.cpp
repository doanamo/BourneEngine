#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Platform/Time.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Graphics/GraphicsSystem.hpp"
#include "Engine/Graphics/GraphicsStats.hpp"

int main(const int argc, const char* const* argv)
{
    Engine::Setup(argc, argv);

    Platform::Window window;
    const auto windowTitle = InlineString<64>::Format("Bourne Engine %s", BuildVersion::Readable);
    if(!window.Open(windowTitle.GetData(), 1024, 576))
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

        window.ProcessEvents();
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
