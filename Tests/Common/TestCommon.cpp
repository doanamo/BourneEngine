#include "Shared.hpp"
#include "TestCommon.hpp"

TestResult Common::TestCommon()
{
    TEST_SUCCESS(TestUtility());
    TEST_SUCCESS(TestArray());
    return TestResult::Success;
}