#pragma once

enum class TestResult
{
    Unknown,
    Passed,
    Failed,
};

#define TEST_TRUE(expression) \
    if(!(expression)) \
    { \
        return TestResult::Failed; \
    }

#define TEST_FALSE(expression) \
    if(expression) \
    { \
        return TestResult::Failed; \
    }

namespace Testing
{
    bool Setup();
}
