#pragma once

#include "Common/Debug/Debug.hpp"

namespace Test
{
    enum class Result
    {
        Success,
        Failure,
    };

    void SetCurrentTestResult(Result result);
    Result GetCurrentTestResult();
}

#define TEST_TRUE(expression) \
    if(!(expression)) \
    { \
        Test::SetCurrentTestResult(Test::Result::Failure); \
        LOG_ERROR("Test true assertion failed: %s", #expression); \
        DEBUG_BREAK(); \
        return; \
    }

#define TEST_FALSE(expression) \
    if(expression) \
    { \
        Test::SetCurrentTestResult(Test::Result::Failure); \
        LOG_ERROR("Test false assertion failed: %s", #expression); \
        DEBUG_BREAK(); \
        return; \
    }
