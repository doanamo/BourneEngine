#include "Shared.hpp"
#include "TestCommon.hpp"

TestResult Common::RunTests()
{
    TEST_SUCCESS(TestUtility());
    TEST_SUCCESS(TestFunction());
    TEST_SUCCESS(TestUniquePtr());
    TEST_SUCCESS(TestArray());
    TEST_SUCCESS(TestString());
    TEST_SUCCESS(TestStringView());
    return TestResult::Success;
}