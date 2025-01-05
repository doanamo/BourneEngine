#pragma once

namespace Test
{
    class MemoryStats final
    {
#ifdef ENABLE_MEMORY_STATS
        i64 m_allocatedTotalCount;
        i64 m_allocatedTotalBytes;
#endif
    public:
        MemoryStats();

        bool ValidateAllocations(i64 count, i64 bytes) const;
    };
}
