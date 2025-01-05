#pragma once

#if defined(PLATFORM_WINDOWS)
    #include "Windows/Defines.hpp"
#elif defined(PLATFORM_LINUX)
    #include "Linux/Defines.hpp"
#else
    #error Unsupported platform!
#endif
