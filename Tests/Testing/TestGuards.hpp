#pragma once

namespace Test
{
    class MemoryGuard final
    {
    #if ENABLE_MEMORY_STATS
        // Initial stats used as baseline for comparison.
        u64 m_initialAllocatedCurrentCount;
        u64 m_initialAllocatedCurrentBytes;
        u64 m_initialAllocatedTotalCount;
        u64 m_initialAllocatedTotalBytes;
        u64 m_initialReallocatedTotalCount;
        u64 m_initialReallocatedTotalBytes;

        // Stats that will be compared at the end of scope.
        u64 m_scopeAllocatedCurrentCount = 0;
        u64 m_scopeAllocatedCurrentBytes = 0;
        u64 m_scopeAllocatedTotalCount = 0;
        u64 m_scopeAllocatedTotalBytes = 0;
        u64 m_scopeReallocatedTotalCount = 0;
        u64 m_scopeReallocatedTotalBytes = 0;
    #endif

    public:
        MemoryGuard();
        ~MemoryGuard();

        struct ValidateParams
        {
            u64 allocationCount = 0;
            u64 allocationBytes = 0;
            u64 reallocationCount = 0;
            u64 reallocationBytes = 0;
            u64 deallocationCount = 0;
            u64 deallocationBytes = 0;
        };

        bool Validate(const ValidateParams& params);

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

        struct ValidateParams
        {
            u64 instances = 0;
            u64 constructions = 0;
            u64 destructions = 0;
            u64 copies = 0;
            u64 moves = 0;
        };

        bool Validate(const ValidateParams& params = {});

        bool ValidateCurrentInstances(u64 count);
        bool ValidateTotalCounts(u64 constructs, u64 destructs, u64 copies, u64 moves);
        bool ValidateTotalConstructs(u64 count);
        bool ValidateTotalDestructs(u64 count);
        bool ValidateTotalCopies(u64 count);
        bool ValidateTotalMoves(u64 count);
    };
}
