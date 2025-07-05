#pragma once

#if CONFIG_DEBUG
    #define CONFIG_NAME "Debug"
#elif CONFIG_DEVELOP
    #define CONFIG_NAME "Develop"
#elif CONFIG_RELEASE
    #define CONFIG_NAME "Release"
#else
    #error Unknown configuration!
#endif

#define ENABLE_ASSERT !CONFIG_RELEASE // Compile asserts in non-Release builds
#define ENABLE_ASSERT_SLOW CONFIG_DEBUG // Compile asserts that are too slow for non-Debug builds

#define ENABLE_LOGGER !CONFIG_RELEASE // Compile logger in non-Release builds
#define ENABLE_LOGGER_SOURCE_LINE CONFIG_DEBUG // Format source file and line in logger messages
#define ENABLE_LOGGER_CONSOLE_OUTPUT !CONFIG_RELEASE // Output to console which is not present in Release
