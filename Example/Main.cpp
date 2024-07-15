#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Graphics/Device.hpp"

int main()
{
    Engine::Setup();

    Platform::Window window;
    if(!window.Open("Example", 1024, 576))
    {
        LOG_FATAL("Failed to setup platform window");
        return -1;
    }

    Graphics::Device device;
    if(!device.Setup(window))
    {
        LOG_FATAL("Failed to setup graphics device");
        return -1;
    }

    while(true)
    {
        window.ProcessEvents();
        if(!window.IsOpen())
            break;

        device.BeginFrame();
        device.EndFrame();
    }

    return 0;
}
