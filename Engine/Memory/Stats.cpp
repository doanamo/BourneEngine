#include "Shared.hpp"
#include "Stats.hpp"

#if ENABLE_MEMORY_STATS

void Memory::Stats::Print() const
{
    LOG_INFO("Memory stats:");
    LOG_NO_SOURCE_LINE_SCOPE();
    LOG_INFO("  Current allocations: %llu (%llu bytes)", GetAllocatedCurrentCount(), GetAllocatedCurrentBytes());
    LOG_INFO("  Current inline allocations: %llu (%llu bytes)", GetInlineAllocatedCurrentCount(), GetInlineAllocatedCurrentBytes());
    LOG_INFO("  Current system allocations: %llu (%llu bytes)", GetSystemAllocatedCurrentCount(), GetSystemAllocatedCurrentBytes());
    LOG_INFO("  Current system header bytes: %llu bytes", GetSystemHeaderCurrentBytes());

    // #todo: Print peak and system stats.
}

void Memory::Stats::OnExit() const
{
    const u64 allocatedCurrentCount = GetAllocatedCurrentCount();
    const u64 allocatedCurrentBytes = GetAllocatedCurrentBytes();
    if(allocatedCurrentCount != 0 || allocatedCurrentBytes != 0)
    {
        LOG_ERROR("Memory leak detected: %lli allocations, %lli bytes",
            allocatedCurrentCount, allocatedCurrentBytes);
    }

    const u64 allocatedTotalCount = GetAllocatedTotalCount();
    const u64 deallocatedTotalCount = GetDeallocatedTotalCount();
    if(allocatedTotalCount != deallocatedTotalCount)
    {
        LOG_ERROR("Memory leak detected: %lli total allocations, %lli total deallocations",
            allocatedTotalCount, deallocatedTotalCount);
    }

    const u64 allocatedTotalBytes = GetAllocatedTotalBytes();
    const u64 deallocatedTotalBytes = GetDeallocatedTotalBytes();
    if(allocatedTotalBytes != deallocatedTotalBytes)
    {
        LOG_ERROR("Memory leak detected: %lli bytes allocated, %lli bytes deallocated",
            allocatedTotalBytes, deallocatedTotalBytes);
    }

    const u64 inlineAllocatedCurrentCount = GetInlineAllocatedCurrentCount();
    const u64 inlineAllocatedCurrentBytes = GetInlineAllocatedCurrentBytes();
    if(inlineAllocatedCurrentCount != 0 || inlineAllocatedCurrentBytes != 0)
    {
        LOG_ERROR("Inline memory leak detected: %lli allocations, %lli bytes",
            inlineAllocatedCurrentCount, inlineAllocatedCurrentBytes);
    }

    const u64 systemAllocatedCurrentCount = GetSystemAllocatedCurrentCount();
    const u64 systemAllocatedCurrentBytes = GetSystemAllocatedCurrentBytes();
    if(systemAllocatedCurrentCount != 0 || systemAllocatedCurrentBytes != 0)
    {
        LOG_ERROR("System memory leak detected: %lli allocations, %lli bytes",
            systemAllocatedCurrentCount, systemAllocatedCurrentBytes);
    }

    const u64 systemHeaderCurrentBytes = GetSystemHeaderCurrentBytes();
    if(systemHeaderCurrentBytes != 0)
    {
        LOG_ERROR("System header memory leak detected: %lli bytes",
            systemHeaderCurrentBytes);
    }
}

void Memory::Stats::OnAllocation(const u64 size)
{
    m_allocatedCurrentCount.fetch_add(1, std::memory_order_relaxed);
    m_allocatedCurrentBytes.fetch_add(size, std::memory_order_relaxed);
    m_allocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    m_allocatedTotalBytes.fetch_add(size, std::memory_order_relaxed);
}

void Memory::Stats::OnReallocation(const u64 newSize, const u64 oldSize)
{
    m_reallocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    m_reallocatedTotalBytes.fetch_add(oldSize, std::memory_order_relaxed);
    
    if(newSize > oldSize)
    {
        m_allocatedCurrentBytes.fetch_add(newSize - oldSize, std::memory_order_relaxed);
        m_allocatedTotalBytes.fetch_add(newSize - oldSize, std::memory_order_relaxed);
    }
    else
    {
        m_allocatedCurrentBytes.fetch_sub(oldSize - newSize, std::memory_order_relaxed);
        m_deallocatedTotalBytes.fetch_add(oldSize - newSize, std::memory_order_relaxed);
    }

    ASSERT_SLOW(m_allocatedCurrentBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnDeallocation(const u64 size)
{
    m_deallocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    m_deallocatedTotalBytes.fetch_add(size, std::memory_order_relaxed);
    m_allocatedCurrentCount.fetch_sub(1, std::memory_order_relaxed);
    m_allocatedCurrentBytes.fetch_sub(size, std::memory_order_relaxed);
    ASSERT_SLOW(m_allocatedCurrentCount.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(m_allocatedCurrentBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnInlineAllocation(const u64 size)
{
    m_inlineAllocatedCurrentCount.fetch_add(1, std::memory_order_relaxed);
    m_inlineAllocatedCurrentBytes.fetch_add(size, std::memory_order_relaxed);
}

void Memory::Stats::OnInlineDeallocation(const u64 size)
{
    m_inlineAllocatedCurrentCount.fetch_sub(1, std::memory_order_relaxed);
    m_inlineAllocatedCurrentBytes.fetch_sub(size, std::memory_order_relaxed);
    ASSERT_SLOW(m_inlineAllocatedCurrentCount.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(m_inlineAllocatedCurrentBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnSystemAllocation(const u64 size, const u64 headerSize)
{
    m_systemAllocatedCurrentCount.fetch_add(1, std::memory_order_relaxed);
    m_systemAllocatedCurrentBytes.fetch_add(size, std::memory_order_relaxed);
    m_systemHeaderCurrentBytes.fetch_add(headerSize, std::memory_order_relaxed);
}

void Memory::Stats::OnSystemReallocation(const u64 newSize, const u64 oldSize)
{
    if(newSize > oldSize)
    {
        m_systemAllocatedCurrentBytes.fetch_add(newSize - oldSize, std::memory_order_relaxed);
    }
    else
    {
        m_systemAllocatedCurrentBytes.fetch_sub(oldSize - newSize, std::memory_order_relaxed);
    }

    ASSERT_SLOW(m_systemAllocatedCurrentBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnSystemDeallocation(const u64 size, const u64 headerSize)
{
    m_systemAllocatedCurrentCount.fetch_sub(1, std::memory_order_relaxed);
    m_systemAllocatedCurrentBytes.fetch_sub(size, std::memory_order_relaxed);
    m_systemHeaderCurrentBytes.fetch_sub(headerSize, std::memory_order_relaxed);
    ASSERT_SLOW(m_systemAllocatedCurrentCount.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(m_systemAllocatedCurrentBytes.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(m_systemHeaderCurrentBytes.load(std::memory_order_relaxed) >= 0);
}

#endif
