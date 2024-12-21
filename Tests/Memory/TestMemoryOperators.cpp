#include "Shared.hpp"
#include "TestMemory.hpp"
#include "TestMemoryStats.hpp"

TestResult Memory::TestOperators()
{
    LOG_INFO("Running Memory::TestOperators...");
    Memory::TestStats memoryStats;

    // Note: Keyword volatile is used here to stop Clang from optimizing out
    // the allocations, which we need here for unit testing purposes.
    // See: https://godbolt.org/z/r65fsf77W

    // Test operator new
    {
        volatile u32* value = new u32(42);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32)));
        TEST_TRUE(value != nullptr);
        TEST_TRUE(*value == 42);

        delete value;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    // Test operator new array
    {
        volatile u32* values = new u32[4]{ 1, 2, 3, 4 };
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