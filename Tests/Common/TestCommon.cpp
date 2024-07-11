#include "Shared.hpp"
#include "TestCommon.hpp"

TestResult Common::RunTests()
{
    TEST_SUCCESS(TestUtility());
    TEST_SUCCESS(TestArray());
    TEST_SUCCESS(TestUniquePtr());
    return TestResult::Success;
}