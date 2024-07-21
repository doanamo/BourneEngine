#include "Shared.hpp"
#include "TestMemory.hpp"

TestResult Memory::TestOperators()
{
#ifndef CONFIG_RELEASE
    u64 baseAllocationCount = DefaultAllocator::GetAllocationCount();
    u64 baseAllocatedBytes = DefaultAllocator::GetAllocatedUsableBytes();
#endif

    // Test operator new
    {
        u32* value = new u32(42);
        TEST_TRUE(value != nullptr);
        TEST_TRUE(*value == 42);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32));
    #endif

        delete value;

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif
    }

    // Test operator new array
    {
        u32* values = new u32[4]{ 1, 2, 3, 4 };
        TEST_TRUE(values != nullptr);
        TEST_TRUE(values[0] == 1);
        TEST_TRUE(values[1] == 2);
        TEST_TRUE(values[2] == 3);
        TEST_TRUE(values[3] == 4);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 4);
    #endif

        delete[] values;

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif
    }

    return TestResult::Success;
}