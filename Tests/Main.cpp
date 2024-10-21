#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Common/TestCommon.hpp"
#include "Memory/TestMemory.hpp"

TestResult RunTests()
{
    LOG_INFO("Running all tests...");
    Memory::ScopedStats memoryStats;

    {
        TEST_SUCCESS(Memory::RunTests());
        TEST_SUCCESS(Common::RunTests());
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    return TestResult::Success;
}

int main()
{
    // #todo: Integrate with CTest for CI build and see if VS support is suitable.
    // #future: Write Visual Studio test adapter extension

    Engine::Setup();

    if(RunTests() != TestResult::Success)
    {
        LOG_ERROR("Test execution has failed");
        return -1;
    }

    LOG_SUCCESS("All tests passed successfully");
    return 0;
}
