#include "Shared.hpp"
#include "Memory/MemoryStats.hpp"
#include "TestMemoryStats.hpp"

Memory::TestStats::TestStats()
{
#ifdef ENABLE_MEMORY_STATS
    m_systemAllocatedTotalCount = Stats::GetSystemAllocatedTotalCount();
    m_systemAllocatedUsableBytes = Stats::GetSystemAllocatedUsableBytes();
#endif
}

bool Memory::TestStats::ValidateSystemAllocations(i64 count, i64 bytes) const
{
#ifdef ENABLE_MEMORY_STATS
    if(Stats::GetSystemAllocatedTotalCount() != m_systemAllocatedTotalCount + count)
        return false;

    if(Stats::GetSystemAllocatedUsableBytes() != m_systemAllocatedUsableBytes + bytes)
        return false;
#endif

    return true;
}
