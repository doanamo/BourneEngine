#include "Shared.hpp"

TEST_DEFINE("Memory.Allocators.Inline", "String")
{
    InlineString<8> string = "Hello!!";
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Memory.Allocators.Inline", "StringAppend")
{
    InlineString<12> string;
    string += "Hello ";
    string += "world";
    string += "!";
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1));
}

TEST_DEFINE("Memory.Allocators.Inline", "Array")
{
    InlineArray<u32, 2> array = { 4, 2 };
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Memory.Allocators.Inline", "ArrayAppend")
{
    InlineArray<u32, 2> array;
    array.Add(4);
    array.Add(2);
    array.Add(69);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1));
}

TEST_DEFINE("Memory.Allocators.Inline", "EmptyString")
{
    InlineString<16> string;
    TEST_TRUE(string.GetCapacity() == 15);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Memory.Allocators.Inline", "EmptyHeapString")
{
    HeapString string;
    TEST_TRUE(string.GetCapacity() == 0);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Memory.Allocators.Inline", "EmptyArray")
{
    InlineArray<u32, 16> array;
    TEST_TRUE(array.GetCapacity() == 16);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Memory.Allocators.Inline", "EmptyHeapArray")
{
    HeapArray<u32> array;
    TEST_TRUE(array.GetCapacity() == 0);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}
