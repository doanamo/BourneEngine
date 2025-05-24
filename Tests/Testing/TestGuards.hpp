#pragma once

namespace Test
{
    class MemoryGuard final
    {
    #if ENABLE_MEMORY_STATS
        u64 m_allocatedTotalCount;
        u64 m_allocatedTotalBytes;
        u64 m_allocatedCurrentCount;
        u64 m_allocatedCurrentBytes;
    #endif

    public:
        MemoryGuard();
        ~MemoryGuard();

        bool ValidateTotalAllocations(u64 count) const;
        bool ValidateTotalAllocations(u64 count, u64 bytes) const;

        bool ValidateCurrentAllocations(u64 count) const;
        bool ValidateCurrentAllocations(u64 count, u64 bytes) const;
    };

    class ObjectGuard final
    {
        u64 m_instanceCurrentCount;
        u64 m_constructTotalCount;
        u64 m_destructTotalCount;
        u64 m_copyTotalCount;
        u64 m_moveTotalCount;

    public:
        ObjectGuard();
        ~ObjectGuard();

        bool ValidateCurrentInstances(u64 count);
        bool ValidateTotalCounts(u64 constructs, u64 destructs, u64 copies, u64 moves);
        bool ValidateTotalConstructs(u64 count);
        bool ValidateTotalDestructs(u64 count);
        bool ValidateTotalCopies(u64 count);
        bool ValidateTotalMoves(u64 count);
    };
}
