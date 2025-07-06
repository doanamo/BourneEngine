#pragma once

#include "Common/Config.hpp"
#include "Platform/Config.hpp"
#include "Graphics/Config.hpp"

// #todo: Document each config variable.
struct Config
{
    bool headless = false;

    Common::LoggerConfig logger;
    Platform::WindowConfig window;
    Graphics::SystemConfig graphics;
};
