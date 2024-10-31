#pragma once

#ifdef ENABLE_MEMORY_STATS

namespace Memory
{
    class Stats final
    {
    private:
        static std::atomic<i64> s_allocatedTotalCount;
        static std::atomic<i64> s_allocatedTotalBytes;

        static std::atomic<i64> s_systemAllocatedTotalCount;
        static std::atomic<i64> s_systemAllocatedTotalBytes;
        static std::atomic<i64> s_systemAllocatedHeaderBytes;
        static std::atomic<i64> s_systemAllocatedUsableBytes;

    public:
        Stats() = delete;

        static void OnAllocation(u64 allocationSize);
        static void OnReallocation(i64 sizeDifference);
        static void OnDeallocation(u64 allocationSize);

        static void OnSystemAllocation(u64 allocationSize, u64 headerSize);
        static void OnSystemReallocation(i64 sizeDifference);
        static void OnSystemDeallocation(u64 allocationSize, u64 headerSize);

        static i64 GetAllocatedTotalCount();
        static i64 GetAllocatedTotalBytes();

        static i64 GetSystemAllocatedTotalCount();
        static i64 GetSystemAllocatedTotalBytes();
        static i64 GetSystemAllocatedHeaderBytes();
        static i64 GetSystemAllocatedUsableBytes();
    };
}

#endif // ENABLE_MEMORY_STATS
