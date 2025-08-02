#pragma once

#if ENABLE_MEMORY_STATS

#include "Common/Utility/Singleton.hpp"

namespace Memory
{
    class Stats final : public Singleton<Stats>
    {
        // Stats for allocations requested by engine/application.
        // These do not represent the actual memory usage!
        std::atomic<u64> m_allocatedCurrentCount = 0;
        std::atomic<u64> m_allocatedCurrentBytes = 0;

        std::atomic<u64> m_allocatedTotalCount = 0;
        std::atomic<u64> m_allocatedTotalBytes = 0;

        std::atomic<u64> m_reallocatedTotalCount = 0;
        std::atomic<u64> m_reallocatedTotalBytes = 0;

        std::atomic<u64> m_deallocatedTotalCount = 0;
        std::atomic<u64> m_deallocatedTotalBytes = 0;

        // Stats for allocations performed by inline memory allocator.
        // These can be placed in either heap or stack memory.
        std::atomic<u64> m_inlineAllocatedCurrentCount = 0;
        std::atomic<u64> m_inlineAllocatedCurrentBytes = 0;

        // Stats for allocations requested directly from the system.
        // These usually represent memory requested by various allocators.
        std::atomic<u64> m_systemAllocatedCurrentCount = 0;
        std::atomic<u64> m_systemAllocatedCurrentBytes = 0;
        std::atomic<u64> m_systemHeaderCurrentBytes = 0;

        // #todo: Retrieve process memory usage stats from the system.
        // #todo: Track and print peak allocation stats.

    public:
        void Print() const;
        void OnExit() const;

        void OnAllocation(u64 size);
        void OnReallocation(u64 newSize, u64 oldSize);
        void OnDeallocation(u64 size);

        void OnInlineAllocation(u64 size);
        void OnInlineDeallocation(u64 size);

        void OnSystemAllocation(u64 size, u64 headerSize);
        void OnSystemReallocation(u64 newSize, u64 oldSize);
        void OnSystemDeallocation(u64 size, u64 headerSize);

        u64 GetAllocatedCurrentCount() const
        {
            return m_allocatedCurrentCount.load(std::memory_order_relaxed);
        }

        u64 GetAllocatedCurrentBytes() const
        {
            return m_allocatedCurrentBytes.load(std::memory_order_relaxed);
        }

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

        u64 GetDeallocatedTotalCount() const
        {
            return m_deallocatedTotalCount.load(std::memory_order_relaxed);
        }

        u64 GetDeallocatedTotalBytes() const
        {
            return m_deallocatedTotalBytes.load(std::memory_order_relaxed);
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

        u64 GetSystemHeaderCurrentBytes() const
        {
            return m_systemHeaderCurrentBytes.load(std::memory_order_relaxed);
        }
    };
}

#endif
