#pragma once

namespace Test
{
    class MemoryStats final
    {
    #if ENABLE_MEMORY_STATS
        u64 m_allocatedCurrentCount;
        u64 m_allocatedCurrentBytes;
    #endif

    public:
        MemoryStats();

        bool ValidateAllocations(u64 count, u64 bytes) const;
    };
}
