#include "Shared.hpp"
#include "Platform/Time.hpp"
#include "Includes.hpp"

namespace Time
{
    u64 ReadTickFrequency()
    {
        LARGE_INTEGER result;
        ASSERT_EVALUATE(QueryPerformanceFrequency(&result));
        ASSERT(result.QuadPart != 0);
        return result.QuadPart;
    }

    const u64 g_tickFrequency = ReadTickFrequency();
}

u64 Time::GetTickFrequency()
{
    return g_tickFrequency;
}

u64 Time::GetCurrentTick()
{
    LARGE_INTEGER result;
    ASSERT_EVALUATE(QueryPerformanceCounter(&result));
    return result.QuadPart;
}
