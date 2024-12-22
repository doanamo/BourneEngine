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

void Memory::Stats::OnAllocation(const u64 size)
{
    s_allocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    s_allocatedTotalBytes.fetch_add(size, std::memory_order_relaxed);
}

void Memory::Stats::OnReallocation(const u64 newSize, const u64 oldSize)
{
    s_allocatedTotalBytes.fetch_add(static_cast<i64>(newSize) - oldSize, std::memory_order_relaxed);
    ASSERT_SLOW(s_allocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnDeallocation(const u64 size)
{
    s_allocatedTotalCount.fetch_sub(1, std::memory_order_relaxed);
    s_allocatedTotalBytes.fetch_sub(size, std::memory_order_relaxed);
    ASSERT_SLOW(s_allocatedTotalCount.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(s_allocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnSystemAllocation(const u64 size, const u64 headerSize)
{
    s_systemAllocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    s_systemAllocatedTotalBytes.fetch_add(size, std::memory_order_relaxed);
    s_systemAllocatedHeaderBytes.fetch_add(headerSize, std::memory_order_relaxed);
}

void Memory::Stats::OnSystemReallocation(const u64 newSize, const u64 oldSize)
{
    s_systemAllocatedTotalBytes.fetch_add(static_cast<i64>(newSize) - oldSize, std::memory_order_relaxed);
    ASSERT_SLOW(s_systemAllocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnSystemDeallocation(const u64 size, const u64 headerSize)
{
    s_systemAllocatedTotalCount.fetch_sub(1, std::memory_order_relaxed);
    s_systemAllocatedTotalBytes.fetch_sub(size, std::memory_order_relaxed);
    s_systemAllocatedHeaderBytes.fetch_sub(headerSize, std::memory_order_relaxed);
    ASSERT_SLOW(s_systemAllocatedTotalCount.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(s_systemAllocatedTotalBytes.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(s_systemAllocatedHeaderBytes.load(std::memory_order_relaxed) >= 0);
}

#endif // ENABLE_MEMORY_STATS
