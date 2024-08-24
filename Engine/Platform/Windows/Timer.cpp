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

Platform::TimeSlice::TimeSlice(u64 startTicks, u64 endTicks)
    : m_startTicks(startTicks)
    , m_endTicks(endTicks)
{
    ASSERT(startTicks <= endTicks);
}

float Platform::TimeSlice::CalculateOverlap(const TimeSlice& range) const
{
    // Calculates how much of given slice is in another slice.
    // Returns 0.0f if no overlap, 1.0f if completely inside.
    if(m_startTicks >= range.m_endTicks || m_endTicks <= range.m_startTicks)
    {
        return 0.0f;
    }

    u64 startTicks = Max(m_startTicks, range.m_startTicks);
    u64 endTicks = std::min(m_endTicks, range.m_endTicks);
    u64 durationTicks = endTicks - startTicks;

    float overlapRatio = (float)durationTicks / GetDurationTicks();
    ASSERT(overlapRatio >= 0.0f && overlapRatio <= 1.0f);
    return overlapRatio;
}

float Platform::TimeSlice::GetDurationSeconds() const
{
    return Timer::ConvertTicksToSeconds(GetDurationTicks());
}

u64 Platform::TimeSlice::GetDurationTicks() const
{
    return m_endTicks - m_startTicks;
}

Platform::Timer::Timer()
{
    Reset();
}

Platform::Timer::~Timer() = default;

float Platform::Timer::Tick()
{
    m_previousTicks = m_currentTicks;
    m_currentTicks = ReadTicks();
    ASSERT(m_currentTicks >= m_previousTicks);
    return GetDeltaSeconds();
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
