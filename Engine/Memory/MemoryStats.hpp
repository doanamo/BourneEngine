#pragma once

#ifdef ENABLE_MEMORY_STATS

namespace Memory
{
    class Stats final
    {
        static std::atomic<i64> s_allocatedTotalCount;
        static std::atomic<i64> s_allocatedTotalBytes;

        static std::atomic<i64> s_inlineAllocatedTotalCount;
        static std::atomic<i64> s_inlineAllocatedTotalBytes;

        static std::atomic<i64> s_systemAllocatedTotalCount;
        static std::atomic<i64> s_systemAllocatedTotalBytes;
        static std::atomic<i64> s_systemAllocatedHeaderBytes;

    public:
        Stats() = delete;

        static void OnAllocation(u64 size);
        static void OnReallocation(u64 newSize, u64 oldSize);
        static void OnDeallocation(u64 size);

        static void OnInlineAllocation(u64 size);
        static void OnInlineDeallocation(u64 size);

        static void OnSystemAllocation(u64 size, u64 headerSize);
        static void OnSystemReallocation(u64 newSize, u64 oldSize);
        static void OnSystemDeallocation(u64 size, u64 headerSize);

        static i64 GetAllocatedTotalCount()
        {
            return s_allocatedTotalCount.load(std::memory_order_relaxed);
        }

        static i64 GetAllocatedTotalBytes()
        {
            return s_allocatedTotalBytes.load(std::memory_order_relaxed);
        }

        static i64 GetInlineAllocatedTotalCount()
        {
            return s_inlineAllocatedTotalCount.load(std::memory_order_relaxed);
        }

        static i64 GetInlineAllocatedTotalBytes()
        {
            return s_inlineAllocatedTotalBytes.load(std::memory_order_relaxed);
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
