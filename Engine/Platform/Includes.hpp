#pragma once

#if defined(PLATFORM_WINDOWS)
    #include "Windows/Includes.hpp"
#elif defined(PLATFORM_LINUX)
    #include "Linux/Includes.hpp"
#else
    #error "Unknown platform define!"
#endif
