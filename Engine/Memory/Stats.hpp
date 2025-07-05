#pragma once

#if ENABLE_MEMORY_STATS

namespace Memory
{
    class Stats final
    {
        static Stats s_instance;

        std::atomic<u64> m_allocatedTotalCount = 0;
        std::atomic<u64> m_allocatedTotalBytes = 0;

        std::atomic<u64> m_reallocatedTotalCount = 0;
        std::atomic<u64> m_reallocatedTotalBytes = 0;

        std::atomic<u64> m_allocatedCurrentCount = 0;
        std::atomic<u64> m_allocatedCurrentBytes = 0;

        std::atomic<u64> m_inlineAllocatedCurrentCount = 0;
        std::atomic<u64> m_inlineAllocatedCurrentBytes = 0;

        std::atomic<u64> m_systemAllocatedCurrentCount = 0;
        std::atomic<u64> m_systemAllocatedCurrentBytes = 0;
        std::atomic<u64> m_systemAllocatedCurrentHeaderBytes = 0;

        Stats() = default;

    public:
        static Stats& Get()
        {
            return s_instance;
        }

        void LogMemoryLeaks();

        void OnAllocation(u64 size);
        void OnReallocation(u64 newSize, u64 oldSize);
        void OnDeallocation(u64 size);

        void OnInlineAllocation(u64 size);
        void OnInlineDeallocation(u64 size);

        void OnSystemAllocation(u64 size, u64 headerSize);
        void OnSystemReallocation(u64 newSize, u64 oldSize);
        void OnSystemDeallocation(u64 size, u64 headerSize);

        void ResetTotalAllocations();

        u64 GetAllocatedTotalCount() const
        {
            return m_allocatedTotalCount.load(std::memory_order_relaxed);
        }

        u64 GetAllocatedTotalBytes() const
        {
            return m_allocatedTotalBytes.load(std::memory_order_relaxed);
        }

        u64 GetReallocatedTotalCount() const
        {
            return m_reallocatedTotalCount.load(std::memory_order_relaxed);
        }

        u64 GetReallocatedTotalBytes() const
        {
            return m_reallocatedTotalBytes.load(std::memory_order_relaxed);
        }

        u64 GetAllocatedCurrentCount() const
        {
            return m_allocatedCurrentCount.load(std::memory_order_relaxed);
        }

        u64 GetAllocatedCurrentBytes() const
        {
            return m_allocatedCurrentBytes.load(std::memory_order_relaxed);
        }

        u64 GetInlineAllocatedCurrentCount() const
        {
            return m_inlineAllocatedCurrentCount.load(std::memory_order_relaxed);
        }

        u64 GetInlineAllocatedCurrentBytes() const
        {
            return m_inlineAllocatedCurrentBytes.load(std::memory_order_relaxed);
        }

        u64 GetSystemAllocatedCurrentCount() const
        {
            return m_systemAllocatedCurrentCount.load(std::memory_order_relaxed);
        }

        u64 GetSystemAllocatedCurrentBytes() const
        {
            return m_systemAllocatedCurrentBytes.load(std::memory_order_relaxed);
        }

        u64 GetSystemAllocatedCurrentHeaderBytes() const
        {
            return m_systemAllocatedCurrentHeaderBytes.load(std::memory_order_relaxed);
        }
    };
}

#endif
