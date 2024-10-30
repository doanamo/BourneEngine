#include "Shared.hpp"
#include "MemoryStats.hpp"

#ifdef ENABLE_MEMORY_STATS

static Memory::Stats g_memoryStats;

static class LeakDetector
{
public:
    ~LeakDetector()
    {
        const i64 allocationCount = g_memoryStats.GetAllocationCount();
        const i64 allocationBytes = g_memoryStats.GetAllocatedTotalBytes();
        ASSERT(allocationCount == 0 && allocationBytes == 0,
            "Memory leak detected: %lli allocations, %lli bytes",
            allocationCount, allocationBytes);
    }
} g_leakDetector;

Memory::Stats& Memory::Stats::Get()
{
    return g_memoryStats;
}

void Memory::Stats::OnAllocate(u64 allocationSize, u64 headerSize)
{
    m_allocationCount.fetch_add(1, std::memory_order_relaxed);
    m_allocatedTotalBytes.fetch_add(allocationSize + headerSize, std::memory_order_relaxed);
    m_allocatedHeaderBytes.fetch_add(headerSize, std::memory_order_relaxed);
}

void Memory::Stats::OnReallocate(i64 sizeDifference)
{
    m_allocatedTotalBytes.fetch_add(sizeDifference, std::memory_order_relaxed);
}

void Memory::Stats::OnDeallocate(u64 allocationSize, u64 headerSize)
{
    m_allocationCount.fetch_sub(1, std::memory_order_relaxed);
    m_allocatedTotalBytes.fetch_sub(allocationSize + headerSize, std::memory_order_relaxed);
    m_allocatedHeaderBytes.fetch_sub(headerSize, std::memory_order_relaxed);
}

i64 Memory::Stats::GetAllocationCount()
{
    return m_allocationCount.load(std::memory_order_relaxed);
}

i64 Memory::Stats::GetAllocatedTotalBytes()
{
    return m_allocatedTotalBytes.load(std::memory_order_relaxed);
}

i64 Memory::Stats::GetAllocatedHeaderBytes()
{
    return m_allocatedHeaderBytes.load(std::memory_order_relaxed);
}

i64 Memory::Stats::GetAllocatedUsableBytes()
{
    i64 totalBytes = GetAllocatedTotalBytes();
    i64 headerBytes = GetAllocatedHeaderBytes();
    ASSERT(headerBytes <= totalBytes);
    return totalBytes - headerBytes;
}

#endif // ENABLE_MEMORY_STATS
