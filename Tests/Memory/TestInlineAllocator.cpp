#include "Shared.hpp"
#include "TestMemory.hpp"

Test::Result Memory::TestInlineAllocator()
{
    LOG_INFO("Running Memory::TestInlineAllocator...");
    const Test::MemoryStats memoryStats;

    // Test inline containers do not allocate on heap
    {
        InlineString<8> string1 = "Hello!!";
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        InlineString<12> string2;
        string2 += "Hello ";
        string2 += "world";
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        InlineArray<u32, 2> array1 = { 4, 2 };
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        InlineArray<u32, 2> array2;
        array2.Add(4);
        array2.Add(2);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        array2.Add(69);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32) * 4));
    }

    return Test::Result::Success;
}
