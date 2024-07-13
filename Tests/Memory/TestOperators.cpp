#include "Shared.hpp"
#include "TestMemory.hpp"

TestResult Memory::TestOperators()
{
    u64 allocationCount = DefaultAllocator::GetAllocationCount();
    u64 allocatedBytes = DefaultAllocator::GetAllocatedUsableBytes();

    // Test operator new
    {
        u32* value = new u32(42);
        TEST_TRUE(value != nullptr);
        TEST_TRUE(*value == 42);

        TEST_TRUE(DefaultAllocator::GetAllocationCount() == allocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == allocatedBytes + sizeof(u32));
        delete value;
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == allocationCount);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == allocatedBytes);
    }

    // Test operator new array
    {
        u32* values = new u32[4]{ 1, 2, 3, 4 };
        TEST_TRUE(values != nullptr);
        TEST_TRUE(values[0] == 1);
        TEST_TRUE(values[1] == 2);
        TEST_TRUE(values[2] == 3);
        TEST_TRUE(values[3] == 4);

        TEST_TRUE(DefaultAllocator::GetAllocationCount() == allocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == allocatedBytes + sizeof(u32) * 4);
        delete[] values;
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == allocationCount);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == allocatedBytes);
    }

    return TestResult::Success;
}