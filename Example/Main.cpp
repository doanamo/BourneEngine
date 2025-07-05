#include "Shared.hpp"
#include "Engine/Engine.hpp"

// #todo: All declaration/definition comments should be using JetBrains format and be prettified by IDE.
int main(const int argc, const char* const* argv)
{
    Engine::Config config;
    config.applicationName = "Bourne Engine Example";
    config.commandLineArgv = argv;
    config.commandLineArgc = argc;
    config.platform.window.width = 1024;
    config.platform.window.height = 576;
    config.graphics.useSoftwareRenderer = false;

    Engine::Engine engine;
    if(!engine.Setup(config))
    {
        LOG_FATAL("Failed to setup engine");
        return -1;
    }

    Memory::Allocate<u64>(20);

    // #todo: Pass application instance class.
    return engine.Run();
}
