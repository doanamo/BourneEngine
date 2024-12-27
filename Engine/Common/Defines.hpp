#pragma once

#if defined(CONFIG_DEBUG)
    #define CONFIG_NAME "Debug"
#elif defined(CONFIG_DEVELOP)
    #define CONFIG_NAME "Develop"
#elif defined(CONFIG_RELEASE)
    #define CONFIG_NAME "Release"
#else
    #error Unknown configuration!
#endif

#ifdef CONFIG_DEBUG
    #define ENABLE_ASSERT_SLOW // Compile asserts that are too slow for non-Debug builds
    #define ENABLE_LOGGER_SOURCE_LINE // Format source file and line in logger messages
#endif

#ifndef CONFIG_RELEASE
    #define ENABLE_ASSERT // Compile asserts in non-Release builds
    #define ENABLE_LOGGER // Compile logger in non-Release builds
    #define ENABLE_LOGGER_CONSOLE_OUTPUT // Output to console which is not present in Release
#endif
