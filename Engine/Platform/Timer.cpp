#include "Shared.hpp"
#include "Platform/Timer.hpp"

static class TimerTickFrequency
{
public:
    TimerTickFrequency()
    {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        m_value = frequency.QuadPart;
        ASSERT(m_value != 0);
    }

    u64 GetValue() const
    {
        return m_value;
    }

private:
    u64 m_value;
} g_timerTickFrequency;

u64 Platform::Timer::ReadTicks()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return counter.QuadPart;
}

float Platform::Timer::ConvertTicksToSeconds(u64 ticks)
{
    return (float)ticks / g_timerTickFrequency.GetValue();
}

u64 Platform::Timer::ConvertSecondsToTicks(float seconds)
{
    ASSERT(seconds >= 0.0f);
    return (u64)(seconds * g_timerTickFrequency.GetValue());
}

Platform::TimeSlice Platform::TimeSlice::FromSecondsDuration(float secondsDuration, u64 ticks)
{
    u64 durationTicks = Timer::ConvertSecondsToTicks(std::abs(secondsDuration));
    if(secondsDuration >= 0.0f)
    {
        return TimeSlice(ticks, ticks + durationTicks);
    }
    else
    {
        return TimeSlice(ticks - durationTicks, ticks);
    }
}

Platform::TimeSlice::TimeSlice(u64 startTick, u64 endTick)
    : m_beginTick(startTick)
    , m_endTick(endTick)
{
    ASSERT(startTick <= endTick);
}

float Platform::TimeSlice::CalculateOverlap(const TimeSlice& range) const
{
    // Calculates how much of given slice is in another slice.
    // Returns 0.0f if no overlap, 1.0f if completely contained.
    if(m_beginTick >= range.m_endTick || m_endTick <= range.m_beginTick)
    {
        return 0.0f;
    }

    u64 startTicks = Max(m_beginTick, range.m_beginTick);
    u64 endTicks = std::min(m_endTick, range.m_endTick);
    u64 durationTicks = endTicks - startTicks;

    float overlapRatio = (float)durationTicks / GetTicks();
    ASSERT(overlapRatio >= 0.0f && overlapRatio <= 1.0f);
    return overlapRatio;
}

float Platform::TimeSlice::GetSeconds() const
{
    return Timer::ConvertTicksToSeconds(GetTicks());
}

u64 Platform::TimeSlice::GetTicks() const
{
    return m_endTick - m_beginTick;
}

Platform::Timer::Timer()
{
    Reset();
}

Platform::Timer::~Timer() = default;

Platform::TimeSlice Platform::Timer::Tick()
{
    m_previousTicks = m_currentTicks;
    m_currentTicks = ReadTicks();
    ASSERT(m_currentTicks >= m_previousTicks);
    return GetTimeSlice();
}

void Platform::Timer::Reset()
{
    m_currentTicks = m_previousTicks = ReadTicks();
}

Platform::TimeSlice Platform::Timer::GetTimeSlice()
{
    return TimeSlice(m_previousTicks, m_currentTicks);
}

float Platform::Timer::GetDeltaSeconds() const
{
    return ConvertTicksToSeconds(GetDeltaTicks());
}

float Platform::Timer::GetElapsedSeconds() const
{
    return ConvertTicksToSeconds(GetElapsedTicks());
}

u64 Platform::Timer::GetDeltaTicks() const
{
    return m_currentTicks - m_previousTicks;
}

u64 Platform::Timer::GetElapsedTicks() const
{
    return ReadTicks() - m_currentTicks;
}
