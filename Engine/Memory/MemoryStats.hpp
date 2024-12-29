#pragma once

#ifdef ENABLE_MEMORY_STATS

namespace Memory
{
    class Stats final
    {
        static Stats s_instance;

        std::atomic<i64> m_allocatedTotalCount = 0;
        std::atomic<i64> m_allocatedTotalBytes = 0;

        std::atomic<i64> m_inlineAllocatedTotalCount = 0;
        std::atomic<i64> m_inlineAllocatedTotalBytes = 0;

        std::atomic<i64> m_systemAllocatedTotalCount = 0;
        std::atomic<i64> m_systemAllocatedTotalBytes = 0;
        std::atomic<i64> m_systemAllocatedHeaderBytes = 0;

        Stats() = default;

    public:
        static Stats& Get();

        void OnAllocation(u64 size);
        void OnReallocation(u64 newSize, u64 oldSize);
        void OnDeallocation(u64 size);

        void OnInlineAllocation(u64 size);
        void OnInlineDeallocation(u64 size);

        void OnSystemAllocation(u64 size, u64 headerSize);
        void OnSystemReallocation(u64 newSize, u64 oldSize);
        void OnSystemDeallocation(u64 size, u64 headerSize);

        i64 GetAllocatedTotalCount() const
        {
            return m_allocatedTotalCount.load(std::memory_order_relaxed);
        }

        i64 GetAllocatedTotalBytes() const
        {
            return m_allocatedTotalBytes.load(std::memory_order_relaxed);
        }

        i64 GetInlineAllocatedTotalCount() const
        {
            return m_inlineAllocatedTotalCount.load(std::memory_order_relaxed);
        }

        i64 GetInlineAllocatedTotalBytes() const
        {
            return m_inlineAllocatedTotalBytes.load(std::memory_order_relaxed);
        }

        i64 GetSystemAllocatedTotalCount() const
        {
            return m_systemAllocatedTotalCount.load(std::memory_order_relaxed);
        }

        i64 GetSystemAllocatedTotalBytes() const
        {
            return m_systemAllocatedTotalBytes.load(std::memory_order_relaxed);
        }

        i64 GetSystemAllocatedHeaderBytes() const
        {
            return m_systemAllocatedHeaderBytes.load(std::memory_order_relaxed);
        }
    };
}

#endif // ENABLE_MEMORY_STATS
