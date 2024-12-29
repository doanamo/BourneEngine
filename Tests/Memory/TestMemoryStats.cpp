#include "Shared.hpp"
#include "Memory/MemoryStats.hpp"
#include "TestMemoryStats.hpp"

Memory::TestStats::TestStats()
{
#ifdef ENABLE_MEMORY_STATS
    m_allocatedTotalCount = Stats::Get().GetAllocatedTotalCount();
    m_allocatedTotalBytes = Stats::Get().GetAllocatedTotalBytes();
#endif
}

bool Memory::TestStats::ValidateAllocations(const i64 count, const i64 bytes) const
{
#ifdef ENABLE_MEMORY_STATS
    if(Stats::Get().GetAllocatedTotalCount() != m_allocatedTotalCount + count)
        return false;

    if(Stats::Get().GetAllocatedTotalBytes() != m_allocatedTotalBytes + bytes)
        return false;
#endif

    return true;
}
