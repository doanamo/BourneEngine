#include "Shared.hpp"
#include "TestGuards.hpp"

Test::MemoryGuard::MemoryGuard()
{
#if ENABLE_MEMORY_STATS
    m_initialAllocatedCurrentCount = Memory::Stats::Get().GetAllocatedCurrentCount();
    m_initialAllocatedCurrentBytes = Memory::Stats::Get().GetAllocatedCurrentBytes();
    m_initialAllocatedTotalCount = Memory::Stats::Get().GetAllocatedTotalCount();
    m_initialAllocatedTotalBytes = Memory::Stats::Get().GetAllocatedTotalBytes();
    m_initialReallocatedTotalCount = Memory::Stats::Get().GetReallocatedTotalCount();
    m_initialReallocatedTotalBytes = Memory::Stats::Get().GetReallocatedTotalBytes();
#endif
}

Test::MemoryGuard::~MemoryGuard()
{
    u64 allocatedCurrentCount = Memory::Stats::Get().GetAllocatedCurrentCount();
    if(m_initialAllocatedCurrentCount != allocatedCurrentCount)
    {
        LOG_ERROR("Memory guard detected leak of %llu allocations",
            allocatedCurrentCount - m_initialAllocatedCurrentCount);
        SetCurrentTestResult(Result::Failure);
    }

    u64 allocatedCurrentBytes = Memory::Stats::Get().GetAllocatedCurrentBytes();
    if(m_initialAllocatedCurrentBytes != allocatedCurrentBytes)
    {
        LOG_ERROR("Memory guard detected leak of %llu bytes",
            allocatedCurrentBytes - m_initialAllocatedCurrentBytes);
        SetCurrentTestResult(Result::Failure);
    }

    u64 allocatedTotalCount = Memory::Stats::Get().GetAllocatedTotalCount();
    if(m_initialAllocatedTotalCount + m_scopeAllocatedTotalCount != allocatedTotalCount)
    {
        LOG_ERROR("Memory guard expected %llu total allocations, have %llu",
            m_scopeAllocatedTotalCount, allocatedTotalCount - m_initialAllocatedTotalCount);
        SetCurrentTestResult(Result::Failure);
    }

    u64 allocatedTotalBytes = Memory::Stats::Get().GetAllocatedTotalBytes();
    if(m_initialAllocatedTotalBytes + m_scopeAllocatedTotalBytes != allocatedTotalBytes)
    {
        LOG_ERROR("Memory guard expected %llu total allocated bytes, have %llu",
            m_scopeAllocatedTotalBytes, allocatedTotalBytes - m_initialAllocatedTotalBytes);
        SetCurrentTestResult(Result::Failure);
    }

    u64 reallocatedTotalCount = Memory::Stats::Get().GetReallocatedTotalCount();
    if(m_initialReallocatedTotalCount + m_scopeReallocatedTotalCount != reallocatedTotalCount)
    {
        LOG_ERROR("Memory guard expected %llu total reallocations, have %llu",
            m_scopeReallocatedTotalCount, reallocatedTotalCount - m_initialReallocatedTotalCount);
        SetCurrentTestResult(Result::Failure);
    }

    u64 reallocatedTotalBytes = Memory::Stats::Get().GetReallocatedTotalBytes();
    if(m_initialReallocatedTotalBytes + m_scopeReallocatedTotalBytes != reallocatedTotalBytes)
    {
        LOG_ERROR("Memory guard expected %llu total reallocated bytes, have %llu",
            m_scopeReallocatedTotalBytes, reallocatedTotalBytes - m_initialReallocatedTotalBytes);
        SetCurrentTestResult(Result::Failure);
    }
}

bool Test::MemoryGuard::ValidateTotalAllocations(u64 count) const
{
#if ENABLE_MEMORY_STATS
    u64 allocatedTotalCount = Memory::Stats::Get().GetAllocatedTotalCount();
    allocatedTotalCount -= m_initialAllocatedTotalCount;

    if(allocatedTotalCount != count)
    {
        LOG_ERROR("Memory guard expected %llu total allocations, have %llu", count, allocatedTotalCount);
        return false;
    }
#endif

    return true;
}

bool Test::MemoryGuard::ValidateTotalAllocations(u64 count, u64 bytes) const
{
#if ENABLE_MEMORY_STATS
    if(!ValidateTotalAllocations(count))
        return false;

    u64 allocatedTotalBytes = Memory::Stats::Get().GetAllocatedTotalBytes();
    allocatedTotalBytes -= m_initialAllocatedTotalBytes;

    if(allocatedTotalBytes != bytes)
    {
        LOG_ERROR("Memory guard expected %llu total allocated bytes, have %llu", bytes, allocatedTotalBytes);
        return false;
    }
#endif

    return true;
}

bool Test::MemoryGuard::ValidateCurrentAllocations(u64 count) const
{
#if ENABLE_MEMORY_STATS
    u64 allocatedCurrentCount = Memory::Stats::Get().GetAllocatedCurrentCount() - m_initialAllocatedCurrentCount;
    if(allocatedCurrentCount != count)
    {
        LOG_ERROR("Memory guard expected %llu current allocations, have %llu", count, allocatedCurrentCount);
        return false;
    }
#endif

    return true;
}

bool Test::MemoryGuard::ValidateCurrentAllocations(const u64 count, const u64 bytes) const
{
#if ENABLE_MEMORY_STATS
    if(!ValidateCurrentAllocations(count))
        return false;

    u64 allocatedCurrentBytes = Memory::Stats::Get().GetAllocatedCurrentBytes() - m_initialAllocatedCurrentBytes;
    if(allocatedCurrentBytes != bytes)
    {
        LOG_ERROR("Memory guard expected %llu current allocated bytes, have %llu", bytes, allocatedCurrentBytes);
        return false;
    }
#endif

    return true;
}

bool Test::MemoryGuard::Validate(const Test::MemoryGuard::ValidateParams &params)
{
#if ENABLE_MEMORY_STATS
    m_scopeAllocatedTotalCount += params.allocationCount;
    m_scopeAllocatedTotalBytes += params.allocationBytes;
    m_scopeAllocatedCurrentCount += params.allocationCount;
    m_scopeAllocatedCurrentBytes += params.allocationBytes;

    ASSERT_SLOW(m_scopeAllocatedCurrentCount >= params.deallocationCount);
    ASSERT_SLOW(m_scopeAllocatedCurrentBytes >= params.deallocationBytes);

    m_scopeAllocatedCurrentCount -= params.deallocationCount;
    m_scopeAllocatedCurrentBytes -= params.deallocationBytes;

    u64 allocatedTotalCount = Memory::Stats::Get().GetAllocatedTotalCount();
    if(m_initialAllocatedTotalCount + m_scopeAllocatedTotalCount != allocatedTotalCount)
    {
        LOG_ERROR("Memory guard expected %llu total allocations in scope, have %llu",
            m_scopeAllocatedTotalCount, allocatedTotalCount - m_initialAllocatedTotalCount);
        return false;
    }

    u64 allocatedTotalBytes = Memory::Stats::Get().GetAllocatedTotalBytes();
    if(m_initialAllocatedTotalBytes + m_scopeAllocatedTotalBytes != allocatedTotalBytes)
    {
        LOG_ERROR("Memory guard expected %llu total allocated bytes in scope, have %llu",
            m_scopeAllocatedTotalBytes, allocatedTotalBytes - m_initialAllocatedTotalBytes);
        return false;
    }

    u64 allocatedCurrentCount = Memory::Stats::Get().GetAllocatedCurrentCount();
    if(m_initialAllocatedCurrentCount + m_scopeAllocatedCurrentCount != allocatedCurrentCount)
    {
        LOG_ERROR("Memory guard expected %llu current allocations in scope, have %llu",
            m_scopeAllocatedCurrentCount, allocatedCurrentCount - m_initialAllocatedCurrentCount);
        return false;
    }

    u64 allocatedCurrentBytes = Memory::Stats::Get().GetAllocatedCurrentBytes();
    if(m_initialAllocatedCurrentBytes + m_scopeAllocatedCurrentBytes != allocatedCurrentBytes)
    {
        LOG_ERROR("Memory guard expected %llu current allocated bytes in scope, have %llu",
            m_scopeAllocatedCurrentBytes, allocatedCurrentBytes - m_initialAllocatedCurrentBytes);
        return false;
    }
#endif

    return true;
}

Test::ObjectGuard::ObjectGuard()
{
    m_instanceCurrentCount = Test::Object::GetInstanceCurrentCount();
    m_constructTotalCount = Test::Object::GetConstructTotalCount();
    m_destructTotalCount = Test::Object::GetDestructTotalCount();
    m_copyTotalCount = Test::Object::GetCopyTotalCount();
    m_moveTotalCount = Test::Object::GetMoveTotalCount();
}

Test::ObjectGuard::~ObjectGuard()
{
//    if(!ValidateCurrentInstances(0))
//    {
//        LOG_ERROR("Object guard detected leak in scope");
//        SetCurrentTestResult(Result::Failure);
//    }

    u64 instanceCurrentCount = Object::GetInstanceCurrentCount();
    if(instanceCurrentCount != 0)
    {
        LOG_ERROR("Object guard detected leak of %llu instances", instanceCurrentCount);
        SetCurrentTestResult(Result::Failure);
    }

    if(m_constructTotalCount != m_destructTotalCount)
    {
        LOG_ERROR("Object guard detected construction/destruction mismatch");
        SetCurrentTestResult(Result::Failure);
    }

    u64 constructTotalCount = Object::GetConstructTotalCount();
    if(m_constructTotalCount != constructTotalCount)
    {
        LOG_ERROR("Object guard expected %llu total constructs, have %llu",
            constructTotalCount, m_constructTotalCount);
        SetCurrentTestResult(Result::Failure);
    }

    u64 destructTotalCount = Object::GetDestructTotalCount();
    if(m_destructTotalCount != destructTotalCount)
    {
        LOG_ERROR("Object guard expected %llu total destructs, have %llu",
            destructTotalCount, m_destructTotalCount);
    }

    u64 copyTotalCount = Object::GetCopyTotalCount();
    if(m_copyTotalCount != copyTotalCount)
    {
        LOG_ERROR("Object guard expected %llu total copies, have %llu",
            copyTotalCount, m_copyTotalCount);
        SetCurrentTestResult(Result::Failure);
    }

    u64 moveTotalCount = Object::GetMoveTotalCount();
    if(m_moveTotalCount != moveTotalCount)
    {
        LOG_ERROR("Object guard expected %llu total moves, have %llu",
            moveTotalCount, m_moveTotalCount);
    }
}

bool Test::ObjectGuard::ValidateCurrentInstances(u64 count)
{
    u64 instanceCurrentCount = Object::GetInstanceCurrentCount() - m_instanceCurrentCount;
    if(instanceCurrentCount != count)
    {
        LOG_ERROR("Object guard expected %llu current instances, have %llu", count, instanceCurrentCount);
        return false;
    }

    return true;
}

bool Test::ObjectGuard::ValidateTotalCounts(u64 constructs, u64 destructs, u64 copies, u64 moves)
{
    bool result = true;
    result &= ValidateTotalConstructs(constructs);
    result &= ValidateTotalDestructs(destructs);
    result &= ValidateTotalCopies(copies);
    result &= ValidateTotalMoves(moves);
    return result;
}

bool Test::ObjectGuard::ValidateTotalConstructs(u64 count)
{
    u64 constructTotalCount = Object::GetConstructTotalCount();
    ASSERT(constructTotalCount >= Object::GetDestructTotalCount());
    constructTotalCount -= m_constructTotalCount;

    if(constructTotalCount != count)
    {
        LOG_ERROR("Object guard expected %llu total constructs, have %llu", count, constructTotalCount);
        return false;
    }

    return true;
}

bool Test::ObjectGuard::ValidateTotalDestructs(u64 count)
{
    u64 destructTotalCount = Object::GetDestructTotalCount();
    ASSERT(destructTotalCount <= Object::GetConstructTotalCount());
    destructTotalCount -= m_destructTotalCount;

    if(destructTotalCount != count)
    {
        LOG_ERROR("Object guard expected %llu total destructs, have %llu!", count, destructTotalCount);
        return false;
    }

    return true;
}

bool Test::ObjectGuard::ValidateTotalCopies(u64 count)
{
    u64 copyTotalCount = Object::GetCopyTotalCount() - m_copyTotalCount;
    if(copyTotalCount != count)
    {
        LOG_ERROR("Object guard expected %llu total copies, have %llu!", count, copyTotalCount);
        return false;
    }

    return true;
}

bool Test::ObjectGuard::ValidateTotalMoves(u64 count)
{
    u64 moveTotalCount = Object::GetMoveTotalCount() - m_moveTotalCount;
    if(moveTotalCount != count)
    {
        LOG_ERROR("Object guard expected %llu total moves, have %llu!", count, moveTotalCount);
        return false;
    }

    return true;
}

bool Test::ObjectGuard::Validate(const Test::ObjectGuard::ValidateParams& params)
{
    m_instanceCurrentCount += params.constructions;
    m_constructTotalCount += params.constructions;

    ASSERT_SLOW(m_instanceCurrentCount >= params.destructions);
    m_instanceCurrentCount -= params.destructions;
    m_destructTotalCount += params.destructions;

    m_copyTotalCount += params.copies;
    m_moveTotalCount += params.moves;

    if(m_constructTotalCount != Object::GetConstructTotalCount())
    {
        LOG_ERROR("Object guard expected %llu total constructs in scope, have %llu",
            m_constructTotalCount, Object::GetConstructTotalCount() - m_constructTotalCount);
        return false;
    }

    if(m_destructTotalCount != Object::GetDestructTotalCount())
    {
        LOG_ERROR("Object guard expected %llu total destructs in scope, have %llu",
            m_destructTotalCount, Object::GetDestructTotalCount() - m_destructTotalCount);
        return false;
    }

    if(m_copyTotalCount != Object::GetCopyTotalCount())
    {
        LOG_ERROR("Object guard expected %llu total copies in scope, have %llu",
            m_copyTotalCount, Object::GetCopyTotalCount() - m_copyTotalCount);
        return false;
    }

    if(m_moveTotalCount != Object::GetMoveTotalCount())
    {
        LOG_ERROR("Object guard expected %llu total moves in scope, have %llu",
            m_moveTotalCount, Object::GetMoveTotalCount() - m_moveTotalCount);
    }

    return true;
}
