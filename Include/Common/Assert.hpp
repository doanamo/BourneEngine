#pragma once

#include "Defines.hpp"

void HandleAssert(const char* file, u32 line, const char* message, ...);
typedef void(AssertCallback)(const char* file, u32 line, const char* message, va_list arguments);
void OnAssertCallback(const char* file, u32 line, const char* message, va_list arguments);
void SetAssertCallback(AssertCallback* callback);

#define ASSERT_SIMPLE(expression) \
    if(!(expression)) \
    { \
        HandleAssert(__FILE__, __LINE__, "Assertion faileld: " ## \
        #expression); \
    }

#define ASSERT_MESSAGE(expression, message, ...) \
    if(!(expression)) \
    { \
        HandleAssert(__FILE__, __LINE__, "Assertion faileld: " ## \
        #expression ## " - " ## message, ## __VA_ARGS__); \
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
