#pragma once

#if defined(PLATFORM_WINDOWS)
    #define PLATFORM_NAME "Windows"
    #include "Windows/Defines.hpp"
#elif defined(PLATFORM_LINUX)
    #define PLATFORM_NAME "Linux"
    #include "Linux/Defines.hpp"
#else
    #error Unsupported platform!
#endif
