#include "Shared.hpp"
#include "TestMemory.hpp"

TestResult Memory::TestOperators()
{
    LOG_INFO("Running Memory::TestOperators...");

#ifdef ENABLE_MEMORY_STATS
    u64 baseAllocationCount = Memory::Stats::Get().GetAllocationCount();
    u64 baseAllocatedBytes = Memory::Stats::Get().GetAllocatedUsableBytes();
#endif

    // Test operator new
    {
        u32* value = new u32(42);
        TEST_TRUE(value != nullptr);
        TEST_TRUE(*value == 42);

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32));
#endif

        delete value;

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
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

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 4);
#endif

        delete[] values;

#ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
#endif
    }

    return TestResult::Success;
}