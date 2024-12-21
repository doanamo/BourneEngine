#pragma once

#ifdef ENABLE_MEMORY_STATS

namespace Memory
{
    class Stats final
    {
        static std::atomic<i64> s_allocatedTotalCount;
        static std::atomic<i64> s_allocatedTotalBytes;

        // #todo: Add inline allocation tracking

        static std::atomic<i64> s_systemAllocatedTotalCount;
        static std::atomic<i64> s_systemAllocatedTotalBytes;
        static std::atomic<i64> s_systemAllocatedHeaderBytes;

    public:
        Stats() = delete;

        static void OnAllocation(u64 allocationSize);
        static void OnReallocation(i64 sizeDifference);
        static void OnDeallocation(u64 allocationSize);

        static void OnSystemAllocation(u64 allocationSize, u64 headerSize);
        static void OnSystemReallocation(i64 sizeDifference);
        static void OnSystemDeallocation(u64 allocationSize, u64 headerSize);

        static i64 GetAllocatedTotalCount()
        {
            return s_allocatedTotalCount.load(std::memory_order_relaxed);
        }

        static i64 GetAllocatedTotalBytes()
        {
            return s_allocatedTotalBytes.load(std::memory_order_relaxed);
        }

        static i64 GetSystemAllocatedTotalCount()
        {
            return s_systemAllocatedTotalCount.load(std::memory_order_relaxed);
        }

        static i64 GetSystemAllocatedTotalBytes()
        {
            return s_systemAllocatedTotalBytes.load(std::memory_order_relaxed);
        }

        static i64 GetSystemAllocatedHeaderBytes()
        {
            return s_systemAllocatedHeaderBytes.load(std::memory_order_relaxed);
        }
    };
}

#endif // ENABLE_MEMORY_STATS
