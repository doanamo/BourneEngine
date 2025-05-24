#include "Shared.hpp"

TEST_DEFINE("Memory.Allocators")
{
    // Test inline containers do not allocate on heap
    {
        Test::MemoryGuard memoryGuard;
        InlineString<8> string = "Hello!!";
        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    {
        Test::MemoryGuard memoryGuard;
        InlineString<12> string;
        string += "Hello ";
        string += "world";
        string += "!";
        TEST_TRUE(memoryGuard.ValidateTotalAllocations(1));
    }

    {
        Test::MemoryGuard memoryGuard;
        InlineArray<u32, 2> array = { 4, 2 };
        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    {
        Test::MemoryGuard memoryGuard;
        InlineArray<u32, 2> array;
        array.Add(4);
        array.Add(2);
        array.Add(69);
        TEST_TRUE(memoryGuard.ValidateTotalAllocations(1));
    }

    // Test initial container capacities
    {
        Test::MemoryGuard memoryGuard;
        InlineString<16> string;
        TEST_TRUE(string.GetCapacity() == 15);
        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    {
        Test::MemoryGuard memoryGuard;
        HeapString string;
        TEST_TRUE(string.GetCapacity() == 0);
        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    {
        Test::MemoryGuard memoryGuard;
        InlineArray<u32, 16> array;
        TEST_TRUE(array.GetCapacity() == 16);
        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    {
        Test::MemoryGuard memoryGuard;
        HeapArray<u32> array;
        TEST_TRUE(array.GetCapacity() == 0);
        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    return Test::Result::Success;
}
