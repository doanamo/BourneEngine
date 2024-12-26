#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Common/TestCommon.hpp"
#include "Memory/TestMemory.hpp"
#include "Memory/TestMemoryStats.hpp"

Test::Result RunTests()
{
    LOG_INFO("Running all tests...");
    Memory::TestStats memoryStats;

    {
        TEST_SUCCESS(Memory::RunTests());
        TEST_SUCCESS(Common::RunTests());
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    return Test::Result::Success;
}

int main(const int argc, const char* const* argv)
{
    // #todo: Disable (or hide behind toggle) address sanitizer because it works poorly with the test framework.
    // #todo: Integrate with CTest for CI build and see if VS support is suitable.
    // #future: Write Visual Studio test adapter extension

    Engine::Setup(argc, argv);

    if(RunTests() != Test::Result::Success)
    {
        LOG_ERROR("Test execution has failed");
        return -1;
    }

    LOG_SUCCESS("All tests passed successfully");
    return 0;
}
