#pragma once

#include "Common/Debug/Debug.hpp"

enum class TestResult
{
    Unknown,
    Success,
    Failure,
};

#define TEST_SUCCESS(expression) \
    if(expression != TestResult::Success) \
    { \
        return TestResult::Failure; \
    }

#define TEST_TRUE(expression) \
    if(!(expression)) \
    { \
        Debug::Break(); \
        return TestResult::Failure; \
    }

#define TEST_FALSE(expression) \
    if(expression) \
    { \
        Debug::Break(); \
        return TestResult::Failure; \
    }
