#include "Shared.hpp"

TEST_DEFINE("Memory.Allocators.Inline", "String")
{
    Test::MemoryGuard memoryGuard;
    InlineString<8> string = "Hello!!";
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Memory.Allocators.Inline", "StringAppend")
{
    Test::MemoryGuard memoryGuard;
    InlineString<12> string;
    string += "Hello ";
    string += "world";
    string += "!";
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1));

    return Test::Result::Success;
}

TEST_DEFINE("Memory.Allocators.Inline", "Array")
{
    Test::MemoryGuard memoryGuard;
    InlineArray<u32, 2> array = { 4, 2 };
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Memory.Allocators.Inline", "ArrayAppend")
{
    Test::MemoryGuard memoryGuard;
    InlineArray<u32, 2> array;
    array.Add(4);
    array.Add(2);
    array.Add(69);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1));

    return Test::Result::Success;
}

TEST_DEFINE("Memory.Allocators.Inline", "EmptyString")
{
    Test::MemoryGuard memoryGuard;
    InlineString<16> string;
    TEST_TRUE(string.GetCapacity() == 15);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Memory.Allocators.Inline", "EmptyHeapString")
{
    Test::MemoryGuard memoryGuard;
    HeapString string;
    TEST_TRUE(string.GetCapacity() == 0);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Memory.Allocators.Inline", "EmptyArray")
{
    Test::MemoryGuard memoryGuard;
    InlineArray<u32, 16> array;
    TEST_TRUE(array.GetCapacity() == 16);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Memory.Allocators.Inline", "EmptyHeapArray")
{
    Test::MemoryGuard memoryGuard;
    HeapArray<u32> array;
    TEST_TRUE(array.GetCapacity() == 0);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));

    return Test::Result::Success;
}
