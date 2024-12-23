#include "Shared.hpp"
#include "Memory/MemoryStats.hpp"
#include "TestMemoryStats.hpp"

Memory::TestStats::TestStats()
{
#ifdef ENABLE_MEMORY_STATS
    m_allocatedTotalCount = Stats::GetAllocatedTotalCount();
    m_allocatedTotalBytes = Stats::GetAllocatedTotalBytes();
#endif
}

bool Memory::TestStats::ValidateAllocations(const i64 count, const i64 bytes) const
{
#ifdef ENABLE_MEMORY_STATS
    if(Stats::GetAllocatedTotalCount() != m_allocatedTotalCount + count)
        return false;

    if(Stats::GetAllocatedTotalBytes() != m_allocatedTotalBytes + bytes)
        return false;
#endif

    return true;
}
