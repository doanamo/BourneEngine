#include "Shared.hpp"
#include "Platform/Timer.hpp"

u64 Time::GetTickFrequency()
{
    return 1'000'000'000;
}

u64 Time::GetCurrentTick()
{
    timespec time;
    ASSERT_EVALUATE(clock_gettime(CLOCK_MONOTONIC_RAW, &time) == 0);
    return time.tv_sec * 1'000'000'000 + time.tv_nsec;
}
