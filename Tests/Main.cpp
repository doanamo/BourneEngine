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
    Engine::Setup(argc, argv);

    if(RunTests() != Test::Result::Success)
    {
        LOG_ERROR("Test execution has failed");
        return -1;
    }

    LOG_SUCCESS("All tests passed successfully");
    return 0;
}
