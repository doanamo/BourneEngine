#pragma once

#if defined(_WINDOWS)
    #define PLATFORM_WINDOWS
#else
    #error Unsupported platform!
#endif

#if defined(CMAKE_CONFIG_DEBUG)
    #define CONFIG_DEBUG
    #define CONFIG_NAME "Debug"
#elif defined(CMAKE_CONFIG_DEVELOP)
    #define CONFIG_DEVELOP
    #define CONFIG_NAME "Develop"
#elif defined(CMAKE_CONFIG_RELEASE)
    #define CONFIG_RELEASE
    #define CONFIG_NAME "Release"
#else
    #error Unknown configuration!
#endif

#define EXPAND(x) x
