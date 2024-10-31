#pragma once

#include "Common/Debug/Debug.hpp"
#include "TestObject.hpp"

enum class TestResult
{
    Unknown,
    Success,
    Failure,
};

// #todo: Print test expression failure to log
#define TEST_SUCCESS(expression) \
    if(expression != TestResult::Success) \
    { \
        return TestResult::Failure; \
    }

#define TEST_TRUE(expression) \
    if(!(expression)) \
    { \
        DEBUG_BREAK(); \
        return TestResult::Failure; \
    }

#define TEST_FALSE(expression) \
    if(expression) \
    { \
        DEBUG_BREAK(); \
        return TestResult::Failure; \
    }
