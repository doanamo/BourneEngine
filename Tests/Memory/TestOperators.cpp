#include "Shared.hpp"
#include "TestMemory.hpp"

Test::Result Memory::TestOperators()
{
    LOG_INFO("Running Memory::TestOperators...");
    const Test::MemoryStats memoryStats;

    // Note: Keyword volatile is used here to stop Clang from optimizing out
    // the allocations, which we need here for unit testing purposes.
    // See: https://godbolt.org/z/r65fsf77W

    // Test operator new
    {
        volatile u32* value = new u32(42);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32)));
        TEST_TRUE(value != nullptr);
        TEST_TRUE(*value == 42);

        *value = 17;
        TEST_TRUE(*value == 17);

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

        values[0] = 5;
        values[1] = 6;
        values[2] = 7;
        values[3] = 8;
        TEST_TRUE(values[0] == 5);
        TEST_TRUE(values[1] == 6);
        TEST_TRUE(values[2] == 7);
        TEST_TRUE(values[3] == 8);

        delete[] values;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    // Test operator with alignment
    {
        struct alignas(64) TestStruct
        {
            explicit TestStruct(const u32 value)
                : value(value)
            {
            }

            u32 value;
            u8 padding[60];
        };

        volatile TestStruct* value = new TestStruct(42);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(TestStruct)));
        TEST_TRUE(value != nullptr);
        TEST_TRUE(value->value == 42);

        value->value = 17;
        TEST_TRUE(value->value == 17);

        delete value;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    return Test::Result::Success;
}