#pragma once

#include "Common/Config.hpp"
#include "Platform/Config.hpp"
#include "Graphics/Config.hpp"

// #todo: Document each config variable.
struct Config
{
    bool headless = false;

    Common::Config common;
    Platform::Config platform;
    Graphics::Config graphics;
};
