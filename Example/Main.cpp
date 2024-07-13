#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Platform/Window.hpp"

int main()
{
    Engine::Setup();

    Window window;
    if(!window.Open("Example", 1024, 576))
    {
        LOG_FATAL("Failed to setup window");
        return -1;
    }

    while(true)
    {
        window.ProcessEvents();
        if(!window.IsOpen())
            break;
    }

    return 0;
}
