#pragma once

#include "Defines.hpp"
#include "Debug.hpp"

/*
    Callback
*/

typedef void(AssertCallback)(const char* expression, const char* file, u32 line, const char* message);
void OnAssertCallback(const char* expression, const char* file, u32 line, const char* message);
void SetAssertCallback(AssertCallback* callback);

/*
    Assert
*/

#define ASSERT_SIMPLE(expression) \
    if(!(expression)) \
    { \
        OnAssertCallback(STRINGIFY(expression), __FILE__, __LINE__, nullptr); \
        Debug::Abort(); \
    }

#define ASSERT_MESSAGE(expression, message) \
    if(!(expression)) \
    { \
        OnAssertCallback(STRINGIFY(expression), __FILE__, __LINE__, message); \
        Debug::Abort(); \
    }

#define ASSERT_DEDUCE(arg1, arg2, arg3, ...) arg3
#define ASSERT_CHOOSER(...) EXPAND(ASSERT_DEDUCE(__VA_ARGS__, ASSERT_MESSAGE, ASSERT_SIMPLE))
#define ASSERT_ALWAYS(...) EXPAND(ASSERT_CHOOSER(__VA_ARGS__)(__VA_ARGS__))

#if !defined(CONFIG_RELEASE)
    #define ASSERT(...) ASSERT_ALWAYS(__VA_ARGS__)
    #define ASSERT_EVALUATE(...) ASSERT(__VA_ARGS__)
#else
    #define ASSERT(...) ((void)0)
    #define ASSERT_EVALUATE(expression, ...) (void)(expression)
#endif
