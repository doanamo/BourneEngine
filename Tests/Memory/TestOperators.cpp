#include "Shared.hpp"
#include "TestMemory.hpp"

TestResult Memory::TestOperators()
{
    LOG_INFO("Running Memory::TestOperators...");
    Memory::ScopedStats memoryStats;

    // Test operator new
    {
        u32* value = new u32(42);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32)));
        TEST_TRUE(value != nullptr);
        TEST_TRUE(*value == 42);

        delete value;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    // Test operator new array
    {
        u32* values = new u32[4]{ 1, 2, 3, 4 };
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32) * 4));
        TEST_TRUE(values != nullptr);
        TEST_TRUE(values[0] == 1);
        TEST_TRUE(values[1] == 2);
        TEST_TRUE(values[2] == 3);
        TEST_TRUE(values[3] == 4);

        delete[] values;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    return TestResult::Success;
}