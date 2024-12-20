#pragma once

#if defined(PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h>
    #undef Yield
#elif defined(PLATFORM_LINUX)
    #include <ctime>
    #include <fcntl.h>
#else
    #error Unknown platform!
#endif
