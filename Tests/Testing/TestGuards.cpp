#include "Shared.hpp"
#include "TestGuards.hpp"

Test::MemoryGuard::MemoryGuard()
{
#if ENABLE_MEMORY_STATS
    m_allocatedTotalCount = Memory::Stats::Get().GetAllocatedTotalCount();
    m_allocatedTotalBytes = Memory::Stats::Get().GetAllocatedTotalBytes();
    m_allocatedCurrentCount = Memory::Stats::Get().GetAllocatedCurrentCount();
    m_allocatedCurrentBytes = Memory::Stats::Get().GetAllocatedCurrentBytes();
#endif
}

Test::MemoryGuard::~MemoryGuard()
{
    if(!ValidateCurrentAllocations(0, 0))
    {
        LOG_ERROR("Memory guard detected leak in scope!");
        SetCurrentTestResult(Result::Failure);
    }
}

bool Test::MemoryGuard::ValidateTotalAllocations(u64 count) const
{
#if ENABLE_MEMORY_STATS
    u64 allocatedTotalCount = Memory::Stats::Get().GetAllocatedTotalCount();
    allocatedTotalCount -= m_allocatedTotalCount;

    if(allocatedTotalCount != count)
    {
        LOG_ERROR("Memory guard expected %llu total allocations, have %llu!", count, allocatedTotalCount);
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
    allocatedTotalBytes -= m_allocatedTotalBytes;

    if(allocatedTotalBytes != bytes)
    {
        LOG_ERROR("Memory guard expected %llu total allocated bytes, have %llu!", bytes, allocatedTotalBytes);
        return false;
    }
#endif

    return true;
}

bool Test::MemoryGuard::ValidateCurrentAllocations(u64 count) const
{
#if ENABLE_MEMORY_STATS
    u64 allocatedCurrentCount = Memory::Stats::Get().GetAllocatedCurrentCount() - m_allocatedCurrentCount;
    if(allocatedCurrentCount != count)
    {
        LOG_ERROR("Memory guard expected %llu current allocations, have %llu!", count, allocatedCurrentCount);
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

    u64 allocatedCurrentBytes = Memory::Stats::Get().GetAllocatedCurrentBytes() - m_allocatedCurrentBytes;
    if(allocatedCurrentBytes != bytes)
    {
        LOG_ERROR("Memory guard expected %llu current allocated bytes, have %llu!", bytes, allocatedCurrentBytes);
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
    if(!ValidateCurrentInstances(0))
    {
        LOG_ERROR("Object guard detected leak in scope!");
        SetCurrentTestResult(Result::Failure);
    }

    if(m_constructTotalCount != m_destructTotalCount)
    {
        LOG_ERROR("Object guard detected construction/destruction mismatch in scope!");
        SetCurrentTestResult(Result::Failure);
    }
}

bool Test::ObjectGuard::ValidateCurrentInstances(u64 count)
{
    u64 instanceCurrentCount = Object::GetInstanceCurrentCount() - m_instanceCurrentCount;
    if(instanceCurrentCount != count)
    {
        LOG_ERROR("Object guard expected %llu current instances, have %llu!", count, instanceCurrentCount);
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
        LOG_ERROR("Object guard expected %llu total constructs, have %llu!", count, constructTotalCount);
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
