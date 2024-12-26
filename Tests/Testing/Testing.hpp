#pragma once

#include "Common/Debug/Debug.hpp"
#include "TestObject.hpp"

namespace Test
{
    enum class Result
    {
        Unknown,
        Success,
        Failure,
    };
}

#define TEST_SUCCESS(expression) \
    if(expression != Test::Result::Success) \
    { \
        LOG_ERROR("Test success assertion failed: %s", #expression); \
        return Test::Result::Failure; \
    }

#define TEST_TRUE(expression) \
    if(!(expression)) \
    { \
        LOG_ERROR("Test true assertion failed: %s", #expression); \
        DEBUG_BREAK(); \
        return Test::Result::Failure; \
    }

#define TEST_FALSE(expression) \
    if(expression) \
    { \
        LOG_ERROR("Test false assertion failed: %s", #expression); \
        DEBUG_BREAK(); \
        return Test::Result::Failure; \
    }
