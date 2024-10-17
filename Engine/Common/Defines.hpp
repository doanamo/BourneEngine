#pragma once

#if defined(PLATFORM_WINDOWS)
    #define PLATFORM_NAME "Windows"
#elif defined(PLATFORM_LINUX)
    #define PLATFORM_NAME "Linux"
#elif defined(PLATFORM_MACOS)
    #define PLATFORM_NAME "MacOS"
#else
    #error Unsupported platform!
#endif

#if defined(CONFIG_DEBUG)
    #define CONFIG_NAME "Debug"
#elif defined(CONFIG_DEVELOP)
    #define CONFIG_NAME "Develop"
#elif defined(CONFIG_RELEASE)
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
