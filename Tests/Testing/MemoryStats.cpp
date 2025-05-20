#include "Shared.hpp"
#include "MemoryStats.hpp"

Test::MemoryStats::MemoryStats()
{
#if ENABLE_MEMORY_STATS
    m_allocatedCurrentCount = Memory::Stats::Get().GetAllocatedCurrentCount();
    m_allocatedCurrentBytes = Memory::Stats::Get().GetAllocatedCurrentBytes();
#endif
}

bool Test::MemoryStats::ValidateAllocations(const u64 count, const u64 bytes) const
{
#if ENABLE_MEMORY_STATS
    if(Memory::Stats::Get().GetAllocatedCurrentCount() != m_allocatedCurrentCount + count)
        return false;

    if(Memory::Stats::Get().GetAllocatedCurrentBytes() != m_allocatedCurrentBytes + bytes)
        return false;
#endif

    return true;
}
