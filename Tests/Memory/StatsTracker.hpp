#pragma once

namespace Memory
{
    class StatsTracker final
    {
    public:
        StatsTracker();

        bool ValidateAllocations(i64 count, i64 bytes) const;

    private:
#ifdef ENABLE_MEMORY_STATS
        i64 m_allocationCount;
        i64 m_allocatedBytes;
#endif
    };
}

