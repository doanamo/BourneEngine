#include "Shared.hpp"
#include "TestMemory.hpp"

Test::Result Memory::TestInlineAllocator()
{
    LOG_INFO("Running Memory::TestInlineAllocator...");
    const Test::MemoryStats memoryStats;

    // Test inline containers do not allocate on heap
    {
        InlineString<16> string = "Hello, world!!!";
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        InlineArray<u32, 4> array = { 6, 9, 4, 2 };
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    return Test::Result::Success;
}
