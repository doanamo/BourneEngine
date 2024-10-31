#include "Shared.hpp"
#include "Memory/MemoryStats.hpp"
#include "TestMemoryStats.hpp"

Memory::TestStats::TestStats()
{
#ifdef ENABLE_MEMORY_STATS
    m_allocationCount = Stats::GetAllocatedTotalCount();
    m_allocatedBytes = Stats::GetAllocatedUsableBytes();
#endif
}

bool Memory::TestStats::ValidateAllocations(i64 count, i64 bytes) const
{
#ifdef ENABLE_MEMORY_STATS
    if(Stats::GetAllocatedTotalCount() != m_allocationCount + count)
        return false;

    if(Stats::GetAllocatedUsableBytes() != m_allocatedBytes + bytes)
        return false;
#endif

    return true;
}
