#pragma once

#ifdef ENABLE_MEMORY_STATS

namespace Memory
{
    class Stats final
    {
    private:
        // #todo: rename AllocationTotalCount and AllocationTotalBytes.
        // Use past tense for all allocations done int he past summed.
        // #todo: Make these counters and class static for easier debugging.
        std::atomic<i64> m_allocationCount = 0;
        std::atomic<i64> m_allocatedTotalBytes = 0;
        std::atomic<i64> m_allocatedHeaderBytes = 0;

    public:
        static Stats& Get();

        Stats() = default;
        ~Stats() = default;

        void OnAllocate(u64 allocationSize, u64 headerSize);
        void OnReallocate(i64 sizeDifference);
        void OnDeallocate(u64 allocationSize, u64 headerSize);

        i64 GetAllocationCount();
        i64 GetAllocatedTotalBytes();
        i64 GetAllocatedHeaderBytes();
        i64 GetAllocatedUsableBytes();
    };
}

#endif // ENABLE_MEMORY_STATS
