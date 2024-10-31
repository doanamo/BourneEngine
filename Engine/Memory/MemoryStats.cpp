#include "Shared.hpp"
#include "MemoryStats.hpp"

#ifdef ENABLE_MEMORY_STATS

static class LeakDetector
{
public:
    ~LeakDetector()
    {
        const i64 allocationCount = Memory::Stats::GetAllocatedTotalCount();
        const i64 allocationBytes = Memory::Stats::GetAllocatedTotalBytes();
        ASSERT(allocationCount == 0 && allocationBytes == 0,
            "Memory leak detected: %lli allocations, %lli bytes",
            allocationCount, allocationBytes);
    }
} g_leakDetector;

std::atomic<i64> Memory::Stats::s_allocatedTotalCount = 0;
std::atomic<i64> Memory::Stats::s_allocatedTotalBytes = 0;
std::atomic<i64> Memory::Stats::s_allocatedHeaderBytes = 0;

void Memory::Stats::OnAllocation(u64 allocationSize, u64 headerSize)
{
    s_allocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    s_allocatedTotalBytes.fetch_add(allocationSize + headerSize, std::memory_order_relaxed);
    s_allocatedHeaderBytes.fetch_add(headerSize, std::memory_order_relaxed);
}

void Memory::Stats::OnReallocation(i64 sizeDifference)
{
    s_allocatedTotalBytes.fetch_add(sizeDifference, std::memory_order_relaxed);
}

void Memory::Stats::OnDeallocation(u64 allocationSize, u64 headerSize)
{
    s_allocatedTotalCount.fetch_sub(1, std::memory_order_relaxed);
    s_allocatedTotalBytes.fetch_sub(allocationSize + headerSize, std::memory_order_relaxed);
    s_allocatedHeaderBytes.fetch_sub(headerSize, std::memory_order_relaxed);
}

i64 Memory::Stats::GetAllocatedTotalCount()
{
    return s_allocatedTotalCount.load(std::memory_order_relaxed);
}

i64 Memory::Stats::GetAllocatedTotalBytes()
{
    return s_allocatedTotalBytes.load(std::memory_order_relaxed);
}

i64 Memory::Stats::GetAllocatedHeaderBytes()
{
    return s_allocatedHeaderBytes.load(std::memory_order_relaxed);
}

i64 Memory::Stats::GetAllocatedUsableBytes()
{
    i64 totalBytes = GetAllocatedTotalBytes();
    i64 headerBytes = GetAllocatedHeaderBytes();
    ASSERT(headerBytes <= totalBytes);
    return totalBytes - headerBytes;
}

#endif // ENABLE_MEMORY_STATS
