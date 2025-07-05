#include "Shared.hpp"
#include "Stats.hpp"

#if ENABLE_MEMORY_STATS

Memory::Stats Memory::Stats::s_instance;

void Memory::Stats::LogMemoryLeaks()
{
    const i64 allocationCount = GetAllocatedCurrentCount();
    const i64 allocationBytes = GetAllocatedCurrentBytes();
    if(allocationCount != 0 || allocationBytes != 0)
    {
        LOG_ERROR("Memory leak detected: %lli allocations, %lli bytes",
            allocationCount, allocationBytes);
    }

    const i64 inlineAllocationCount = GetInlineAllocatedCurrentCount();
    const i64 inlineAllocationBytes = GetInlineAllocatedCurrentBytes();
    if(inlineAllocationCount != 0 || inlineAllocationBytes != 0)
    {
        LOG_ERROR("Inline memory leak detected: %lli allocations, %lli bytes",
            inlineAllocationCount, inlineAllocationBytes);
    }

    const i64 systemAllocationCount = GetSystemAllocatedCurrentCount();
    const i64 systemAllocationBytes = GetSystemAllocatedCurrentBytes();
    if(systemAllocationCount != 0 || systemAllocationBytes != 0)
    {
        LOG_ERROR("System memory leak detected: %lli allocations, %lli bytes",
            systemAllocationCount, systemAllocationBytes);
    }

    const i64 systemHeaderBytes = GetSystemAllocatedCurrentHeaderBytes();
    if(systemHeaderBytes != 0)
    {
        LOG_ERROR("System header memory leak detected: %lli bytes",
            systemHeaderBytes);
    }
}

void Memory::Stats::OnAllocation(const u64 size)
{
    m_allocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    m_allocatedTotalBytes.fetch_add(size, std::memory_order_relaxed);
    m_allocatedCurrentCount.fetch_add(1, std::memory_order_relaxed);
    m_allocatedCurrentBytes.fetch_add(size, std::memory_order_relaxed);
}

void Memory::Stats::OnReallocation(const u64 newSize, const u64 oldSize)
{
    m_reallocatedTotalCount.fetch_add(1, std::memory_order_relaxed);
    m_reallocatedTotalBytes.fetch_add(newSize - oldSize, std::memory_order_relaxed);
    m_allocatedCurrentBytes.fetch_add(newSize - oldSize, std::memory_order_relaxed);
    ASSERT_SLOW(m_allocatedCurrentBytes.load(std::memory_order_relaxed) >= 0);

    if(newSize > oldSize)
    {
        m_allocatedTotalBytes.fetch_add(newSize - oldSize, std::memory_order_relaxed);
    }
}

void Memory::Stats::OnDeallocation(const u64 size)
{
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
    m_systemAllocatedCurrentHeaderBytes.fetch_add(headerSize, std::memory_order_relaxed);
}

void Memory::Stats::OnSystemReallocation(const u64 newSize, const u64 oldSize)
{
    m_systemAllocatedCurrentBytes.fetch_add(newSize - oldSize, std::memory_order_relaxed);
    ASSERT_SLOW(m_systemAllocatedCurrentBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::OnSystemDeallocation(const u64 size, const u64 headerSize)
{
    m_systemAllocatedCurrentCount.fetch_sub(1, std::memory_order_relaxed);
    m_systemAllocatedCurrentBytes.fetch_sub(size, std::memory_order_relaxed);
    m_systemAllocatedCurrentHeaderBytes.fetch_sub(headerSize, std::memory_order_relaxed);
    ASSERT_SLOW(m_systemAllocatedCurrentCount.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(m_systemAllocatedCurrentBytes.load(std::memory_order_relaxed) >= 0);
    ASSERT_SLOW(m_systemAllocatedCurrentHeaderBytes.load(std::memory_order_relaxed) >= 0);
}

void Memory::Stats::ResetTotalAllocations()
{
    m_allocatedTotalCount.store(0, std::memory_order_relaxed);
    m_allocatedTotalBytes.store(0, std::memory_order_relaxed);
}

#endif
