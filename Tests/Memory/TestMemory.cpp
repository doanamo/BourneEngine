#include "Shared.hpp"
#include "TestMemory.hpp"

TestResult Memory::RunTests()
{
    TEST_SUCCESS(TestOperators());
    return TestResult::Success;
}

