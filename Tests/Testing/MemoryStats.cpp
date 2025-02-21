#include "Shared.hpp"
#include "MemoryStats.hpp"

Test::MemoryStats::MemoryStats()
{
#if ENABLE_MEMORY_STATS
    m_allocatedTotalCount = Memory::Stats::Get().GetAllocatedTotalCount();
    m_allocatedTotalBytes = Memory::Stats::Get().GetAllocatedTotalBytes();
#endif
}

bool Test::MemoryStats::ValidateAllocations(const i64 count, const i64 bytes) const
{
#if ENABLE_MEMORY_STATS
    if(Memory::Stats::Get().GetAllocatedTotalCount() != m_allocatedTotalCount + count)
        return false;

    if(Memory::Stats::Get().GetAllocatedTotalBytes() != m_allocatedTotalBytes + bytes)
        return false;
#endif

    return true;
}
