#pragma once

#include "Common/Defines.hpp"

#if defined(PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h>
    #undef Yield
#endif
