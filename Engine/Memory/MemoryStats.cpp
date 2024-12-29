#include "Shared.hpp"
#include "MemoryStats.hpp"

#ifdef ENABLE_MEMORY_STATS

namespace Memory
{
    static class LeakDetector
    {
    public:
        ~LeakDetector()
        {
            const i64 allocationCount = Stats::Get().GetAllocatedTotalCount();
            const i64 allocationBytes = Stats::Get().GetAllocatedTotalBytes();
            ASSERT(allocationCount == 0 && allocationBytes == 0,
                "Memory leak detected: %lli allocations, %lli bytes",
                allocationCount, allocationBytes);

            const i64 inlineAllocationCount = Stats::Get().GetInlineAllocatedTotalCount();
            const i64 inlineAllocationBytes = Stats::Get().GetInlineAllocatedTotalBytes();
            ASSERT(inlineAllocationCount == 0 && inlineAllocationBytes == 0,
                "Inline memory leak detected: %lli allocations, %lli bytes",
                inlineAllocationCount, inlineAllocationBytes);

            const i64 systemAllocationCount = Stats::Get().GetSystemAllocatedTotalCount();
            const i64 systemAllocationBytes = Stats::Get().GetSystemAllocatedTotalBytes();
            ASSERT(systemAllocationCount == 0 && systemAllocationBytes == 0,
                "System memory leak detected: %lli allocations, %lli bytes",
                systemAllocationCount, systemAllocationBytes);

            const i64 systemHeaderBytes = Stats::Get().GetSystemAllocatedHeaderBytes();
            ASSERT(systemHeaderBytes == 0, "System header memory leak detected: %lli bytes",
                systemHeaderBytes);
        }
    } g_leakDetector;
}

Memory::Stats Memory::Stats::s_instance;

void Memory::Stats::OnAllocation(const u64 size)
{
    m_allocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    m_allocatedTotalBytes.fetch_add(static_cast<i64>(size), std::memory_order_relaxed);
}

void Memory::Stats::OnReallocation(const u64 newSize, const u64 oldSize)
{
    m_allocatedTotalBytes.fetch_add(static_cast<i64>(newSize - oldSize), std::memory_order_relaxed);
    ASSERT_SLOW(m_allocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnDeallocation(const u64 size)
{
    m_allocatedTotalCount.fetch_sub(1, std::memory_order_relaxed);
    m_allocatedTotalBytes.fetch_sub(static_cast<i64>(size), std::memory_order_relaxed);
    ASSERT_SLOW(m_allocatedTotalCount.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(m_allocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnInlineAllocation(const u64 size)
{
    m_inlineAllocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    m_inlineAllocatedTotalBytes.fetch_add(static_cast<i64>(size), std::memory_order_relaxed);
}

void Memory::Stats::OnInlineDeallocation(const u64 size)
{
    m_inlineAllocatedTotalCount.fetch_sub(1, std::memory_order_relaxed);
    m_inlineAllocatedTotalBytes.fetch_sub(static_cast<i64>(size), std::memory_order_relaxed);
    ASSERT_SLOW(m_inlineAllocatedTotalCount.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(m_inlineAllocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnSystemAllocation(const u64 size, const u64 headerSize)
{
    m_systemAllocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    m_systemAllocatedTotalBytes.fetch_add(static_cast<i64>(size), std::memory_order_relaxed);
    m_systemAllocatedHeaderBytes.fetch_add(static_cast<i64>(headerSize), std::memory_order_relaxed);
}

void Memory::Stats::OnSystemReallocation(const u64 newSize, const u64 oldSize)
{
    m_systemAllocatedTotalBytes.fetch_add(static_cast<i64>(newSize - oldSize), std::memory_order_relaxed);
    ASSERT_SLOW(m_systemAllocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnSystemDeallocation(const u64 size, const u64 headerSize)
{
    m_systemAllocatedTotalCount.fetch_sub(1, std::memory_order_relaxed);
    m_systemAllocatedTotalBytes.fetch_sub(static_cast<i64>(size), std::memory_order_relaxed);
    m_systemAllocatedHeaderBytes.fetch_sub(static_cast<i64>(headerSize), std::memory_order_relaxed);
    ASSERT_SLOW(m_systemAllocatedTotalCount.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(m_systemAllocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(m_systemAllocatedHeaderBytes.load(std::memory_order_relaxed) >= 0);
}

#endif // ENABLE_MEMORY_STATS
