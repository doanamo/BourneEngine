#include "Shared.hpp"
#include "TestCommon.hpp"

TestResult Common::RunTests()
{
    LOG_INFO("Running Common::RunTests...");

    TEST_SUCCESS(TestUtility());
    TEST_SUCCESS(TestArray());
    TEST_SUCCESS(TestString());
    TEST_SUCCESS(TestStringView());
    TEST_SUCCESS(TestUniquePtr());
    return TestResult::Success;
}