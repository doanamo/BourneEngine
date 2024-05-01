#pragma once

/*
    Platforms
*/

#if defined(WIN32)
    #define PLATFORM_WINDOWS
#else
    #error Unsupported platform!
#endif

/*
    Configurations
*/

#if defined(CMAKE_CONFIG_DEBUG)
    #define CONFIG_DEBUG
#elif defined(CMAKE_CONFIG_DEVELOP)
    #define CONFIG_DEVELOP
#elif defined(CMAKE_CONFIG_RELEASE)
    #define CONFIG_RELEASE
#else
    #error Unknown configuration!
#endif

/*
    Macros
*/

#define EXPAND(x) x
#define STRINGIFY(x) #x
