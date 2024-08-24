#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Common/TestCommon.hpp"
#include "Memory/TestMemory.hpp"

TestResult RunTests()
{
    // #future: Write Visual Studio test adapter extension
    TEST_SUCCESS(Memory::RunTests());
    TEST_SUCCESS(Common::RunTests());
    return TestResult::Success;
}

int main()
{
    Engine::Setup();

    if(RunTests() != TestResult::Success)
    {
        LOG_ERROR("Test execution has failed");
        return -1;
    }

    LOG_SUCCESS("All tests passed successfully");
    return 0;
}
