#pragma once

#include "Platform/Config.hpp"
#include "Graphics/Config.hpp"

// #todo: Document each config variable.
namespace Engine
{
    struct Config
    {
        const char* applicationName = "Bourne Engine";
        const char* const* commandLineArgv = nullptr;
        int commandLineArgc = 0;
        bool headless = false;

        Platform::Config platform;
        Graphics::Config graphics;
    };
}