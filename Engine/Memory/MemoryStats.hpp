#pragma once

#ifdef ENABLE_MEMORY_STATS

namespace Memory
{
    class Stats final
    {
    private:
        static std::atomic<i64> s_allocatedTotalCount;
        static std::atomic<i64> s_allocatedTotalBytes;
        static std::atomic<i64> s_allocatedHeaderBytes;

        //static std::atomic<i64> s_systemAllocatedTotalCount;
        //static std::atomic<i64> s_systemAllocatedTotalBytes;

    public:
        Stats() = delete;

        static void OnAllocation(u64 allocationSize, u64 headerSize);
        static void OnReallocation(i64 sizeDifference);
        static void OnDeallocation(u64 allocationSize, u64 headerSize);

        static i64 GetAllocatedTotalCount();
        static i64 GetAllocatedTotalBytes();
        static i64 GetAllocatedHeaderBytes();
        static i64 GetAllocatedUsableBytes();
    };
}

#endif // ENABLE_MEMORY_STATS
