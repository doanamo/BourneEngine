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

    UniquePtr<Platform::Window> window = Platform::Window::Create();
    if(window == nullptr)
    {
        LOG_FATAL("Failed to create platform window");
        return -1;
    }

    UniquePtr<Graphics::System> graphics = Graphics::System::Create(window.Get());
    if(graphics == nullptr)
    {
        LOG_FATAL("Failed to setup graphics system");
        return -1;
    }

    LOG_INFO("Starting main loop...");
    window->Show();

    Time::Timer timer;
    while(true)
    {
        float deltaTime = timer.Tick();

        Platform::Window::ProcessEvents();
        if(window->IsClosing())
            break;

        graphics->BeginFrame();
        {
        }
        graphics->EndFrame();
    }

    LOG_INFO("Exiting application...");
    return 0;
}
