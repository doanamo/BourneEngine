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

        const i64 systemAllocationCount = Memory::Stats::GetSystemAllocatedTotalCount();
        const i64 systemAllocationBytes = Memory::Stats::GetSystemAllocatedTotalBytes();
        ASSERT(systemAllocationCount == 0 && systemAllocationBytes == 0,
            "System memory leak detected: %lli allocations, %lli bytes",
            systemAllocationCount, systemAllocationBytes);

        const i64 systemHeaderBytes = Memory::Stats::GetSystemAllocatedHeaderBytes();
        ASSERT(systemHeaderBytes == 0, "System header memory leak detected: %lli bytes",
            systemHeaderBytes);
    }
} g_leakDetector;

std::atomic<i64> Memory::Stats::s_allocatedTotalCount = 0;
std::atomic<i64> Memory::Stats::s_allocatedTotalBytes = 0;

std::atomic<i64> Memory::Stats::s_systemAllocatedTotalCount = 0;
std::atomic<i64> Memory::Stats::s_systemAllocatedTotalBytes = 0;
std::atomic<i64> Memory::Stats::s_systemAllocatedHeaderBytes = 0;

void Memory::Stats::OnAllocation(u64 allocationSize)
{
    s_allocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    s_allocatedTotalBytes.fetch_add(allocationSize, std::memory_order_relaxed);
}

void Memory::Stats::OnReallocation(i64 sizeDifference)
{
    s_allocatedTotalBytes.fetch_add(sizeDifference, std::memory_order_relaxed);
    ASSERT_SLOW(s_allocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnDeallocation(u64 allocationSize)
{
    s_allocatedTotalCount.fetch_sub(1, std::memory_order_relaxed);
    s_allocatedTotalBytes.fetch_sub(allocationSize, std::memory_order_relaxed);
    ASSERT_SLOW(s_allocatedTotalCount.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(s_allocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnSystemAllocation(u64 allocationSize, u64 headerSize)
{
    s_systemAllocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    s_systemAllocatedTotalBytes.fetch_add(allocationSize, std::memory_order_relaxed);
    s_systemAllocatedHeaderBytes.fetch_add(headerSize, std::memory_order_relaxed);
}

void Memory::Stats::OnSystemReallocation(i64 sizeDifference)
{
    s_systemAllocatedTotalBytes.fetch_add(sizeDifference, std::memory_order_relaxed);
    ASSERT_SLOW(s_systemAllocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnSystemDeallocation(u64 allocationSize, u64 headerSize)
{
    s_systemAllocatedTotalCount.fetch_sub(1, std::memory_order_relaxed);
    s_systemAllocatedTotalBytes.fetch_sub(allocationSize, std::memory_order_relaxed);
    s_systemAllocatedHeaderBytes.fetch_sub(headerSize, std::memory_order_relaxed);
    ASSERT_SLOW(s_systemAllocatedTotalCount.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(s_systemAllocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(s_systemAllocatedHeaderBytes.load(std::memory_order_relaxed) >= 0);
}

#endif // ENABLE_MEMORY_STATS
