#pragma once

namespace Memory
{
    class TestStats final
    {
    public:
        TestStats();

        bool ValidateSystemAllocations(i64 count, i64 bytes) const;

    private:
#ifdef ENABLE_MEMORY_STATS
        i64 m_systemAllocatedTotalCount;
        i64 m_systemAllocatedUsableBytes;
#endif
    };
}
