#include "Shared.hpp"
#include "ScopedStats.hpp"

Memory::ScopedStats::ScopedStats()
{
#ifdef ENABLE_MEMORY_STATS
    m_allocationCount = Stats::Get().GetAllocationCount();
    m_allocatedBytes = Stats::Get().GetAllocatedUsableBytes();
#endif
}

bool Memory::ScopedStats::ValidateAllocations(i64 count, i64 bytes) const
{
#ifdef ENABLE_MEMORY_STATS
    if(Stats::Get().GetAllocationCount() != m_allocationCount + count)
        return false;

    if(Stats::Get().GetAllocatedUsableBytes() != m_allocatedBytes + bytes)
        return false;
#endif

    return true;
}
