#include "Shared.hpp"
#include "StatsTracker.hpp"

Memory::StatsTracker::StatsTracker()
{
#ifdef ENABLE_MEMORY_STATS
    m_allocationCount = Stats::Get().GetAllocationCount();
    m_allocatedBytes = Stats::Get().GetAllocatedUsableBytes();
#endif
}

bool Memory::StatsTracker::ValidateAllocations(i64 count, i64 bytes) const
{
#ifdef ENABLE_MEMORY_STATS
    if(Stats::Get().GetAllocationCount() != m_allocationCount + count)
        return false;

    if(Stats::Get().GetAllocatedUsableBytes() != m_allocatedBytes + bytes)
        return false;
#endif

    return true;
}
