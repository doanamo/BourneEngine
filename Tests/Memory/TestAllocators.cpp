#include "Shared.hpp"

TEST_DEFINE("Memory.Allocators")
{
    const Test::MemoryStats memoryStats;

    // Test inline containers do not allocate on heap
    {
        InlineString<8> string = "Hello!!";
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    {
        InlineString<12> string;
        string += "Hello ";
        string += "world";
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        string += "!";
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(char) * 16));
    }

    {
        InlineArray<u32, 2> array = { 4, 2 };
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    {
        InlineArray<u32, 2> array;
        array.Add(4);
        array.Add(2);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        array.Add(69);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32) * 4));
    }

    // Test initial container capacities
    {
        InlineString<16> string;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
        TEST_TRUE(string.GetCapacity() == 15);
    }

    {
        HeapString string;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
        TEST_TRUE(string.GetCapacity() == 0);
    }

    {
        InlineArray<u32, 16> array;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
        TEST_TRUE(array.GetCapacity() == 16);
    }

    {
        HeapArray<u32> array;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
        TEST_TRUE(array.GetCapacity() == 0);
    }

    return Test::Result::Success;
}
