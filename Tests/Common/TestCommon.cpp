#include "Shared.hpp"
#include "TestCommon.hpp"

Test::Result Common::RunTests()
{
    TEST_SUCCESS(TestUtility());
    TEST_SUCCESS(TestFunction());
    TEST_SUCCESS(TestUniquePtr());
    TEST_SUCCESS(TestArray());
    TEST_SUCCESS(TestString());
    TEST_SUCCESS(TestStringView());
    return Test::Result::Success;
}