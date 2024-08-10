#pragma once

#include "Debug.hpp"

void HandleAssert(const char* file, u32 line, const char* message, ...);

#if !defined(CONFIG_RELEASE)
    #define ASSERT_SOURCE __FILE__
    #define ASSERT_LINE __LINE__
#else
    #define ASSERT_SOURCE nullptr
    #define ASSERT_LINE 0
#endif

#define ASSERT_SIMPLE(expression) \
    if(!(expression)) \
    { \
        HandleAssert(ASSERT_SOURCE, ASSERT_LINE, "Assertion failed: " ## #expression); \
        DEBUG_ABORT(); \
    }

#define ASSERT_MESSAGE(expression, message, ...) \
    if(!(expression)) \
    { \
        HandleAssert(ASSERT_SOURCE, ASSERT_LINE, "Assertion failed: " ## #expression ## " - " ## message, ## __VA_ARGS__); \
        DEBUG_ABORT(); \
    }

#define ASSERT_DEDUCE(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, ...) arg9
#define ASSERT_CHOOSER(...) EXPAND(ASSERT_DEDUCE(__VA_ARGS__, \
    ASSERT_MESSAGE, ASSERT_MESSAGE, ASSERT_MESSAGE, ASSERT_MESSAGE, \
    ASSERT_MESSAGE, ASSERT_MESSAGE, ASSERT_MESSAGE, ASSERT_SIMPLE))
#define ASSERT_ALWAYS(...) EXPAND(ASSERT_CHOOSER(__VA_ARGS__)(__VA_ARGS__))

#if !defined(CONFIG_RELEASE)
    #define ASSERT(...) ASSERT_ALWAYS(__VA_ARGS__)
    #define ASSERT_EVALUATE(...) ASSERT(__VA_ARGS__)
#else
    #define ASSERT(...) ((void)0)
    #define ASSERT_EVALUATE(expression, ...) (void)(expression)
#endif

// #todo: Add slow assert that is only enabled in debug builds
