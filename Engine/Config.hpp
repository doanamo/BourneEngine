#pragma once

#include "Platform/Config.hpp"
#include "Graphics/Config.hpp"

namespace Engine
{
    struct Config
    {
        const char* applicationName = "Bourne Engine";
        const char* const* commandLineArgv = nullptr;
        int commandLineArgc = 0;

        Platform::Config platform;
        Graphics::Config graphics;
    };
}