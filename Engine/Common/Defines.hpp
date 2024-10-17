#pragma once

#if defined(CMAKE_PLATFORM_WINDOWS)
    #define PLATFORM_WINDOWS
    #define PLATFORM_NAME "Windows"
#elif defined(CMAKE_PLATFORM_LINUX)
    #define PLATFORM_LINUX
    #define PLATFORM_NAME "Linux"
#elif defined(CMAKE_PLATFORM_MACOS)
    #define PLATFORM_MACOS
    #define PLATFORM_NAME "macOS"
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

#ifndef CONFIG_RELEASE
    #define ENABLE_ASSERT // Compile asserts in non-Release builds
    #define ENABLE_LOGGER_CONSOLE_OUTPUT // Output to console which is not available in Release
#endif

#ifdef CONFIG_DEBUG
    #define ENABLE_ASSERT_SLOW // Compile asserts that are too slow for non-Debug builds
    #define ENABLE_LOGGER_SOURCE_LINE // Format source file and line in logger messages
#endif
