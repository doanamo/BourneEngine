#include "Shared.hpp"
#include "Platform/Timer.hpp"

u64 Platform::Time::ConvertSecondsToTicks(const float seconds)
{
    ASSERT(seconds >= 0.0f);
    const u64 tickFrequency = GetTickFrequency();
    const double wholeSeconds = std::trunc(seconds);
    const u64 wholeSecondsTicks = static_cast<u64>(wholeSeconds) * tickFrequency;
    const u64 partialSecondTicks = static_cast<u64>((seconds - wholeSeconds) * static_cast<double>(tickFrequency));
    return wholeSecondsTicks + partialSecondTicks;
}

float Platform::Time::ConvertTicksToSeconds(const u64 ticks)
{
    const u64 tickFrequency = GetTickFrequency();
    const u64 wholeSecondsTicks = (ticks / tickFrequency) * tickFrequency;
    const u64 partialSecondTicks = ticks - wholeSecondsTicks;
    const double wholeSeconds = static_cast<double>(wholeSecondsTicks) / static_cast<double>(tickFrequency);
    const double partialSecond = static_cast<double>(partialSecondTicks) / static_cast<double>(tickFrequency);
    return static_cast<float>(wholeSeconds + partialSecond);
}

Platform::TimeSlice Platform::TimeSlice::FromDurationSeconds(const float durationSeconds, const u64 beginTick)
{
    const u64 durationTicks = Time::ConvertSecondsToTicks(std::abs(durationSeconds));
    if(durationSeconds >= 0.0f)
    {
        return {beginTick, beginTick + durationTicks};
    }
    else
    {
        return {beginTick - durationTicks, beginTick};
    }
}

Platform::TimeSlice::TimeSlice(const u64 beginTick, const u64 endTick)
    : m_beginTick(beginTick)
    , m_endTick(endTick)
{
    ASSERT(m_beginTick <= m_endTick);
}

float Platform::TimeSlice::CalculateOverlapSeconds(const TimeSlice& other) const
{
    // Calculates how much of given slice is in another slice.
    // Returns 0.0f if no overlap, 1.0f if completely contained.
    if(m_beginTick >= other.m_endTick || m_endTick <= other.m_beginTick)
    {
        return 0.0f;
    }

    const u64 beginTickMax = std::max(m_beginTick, other.m_beginTick);
    const u64 endTickMin = std::min(m_endTick, other.m_endTick);
    const u64 overlapTicks = endTickMin - beginTickMax;

    const double overlapRatio = static_cast<double>(overlapTicks) / static_cast<double>(GetDurationTicks());
    ASSERT(overlapRatio >= 0.0f && overlapRatio <= 1.0f);
    return static_cast<float>(overlapRatio);
}

u64 Platform::TimeSlice::GetDurationTicks() const
{
    return m_endTick - m_beginTick;
}

float Platform::TimeSlice::GetDurationSeconds() const
{
    return Time::ConvertTicksToSeconds(GetDurationTicks());
}

Platform::Timer::Timer()
{
    Reset();
}

float Platform::Timer::Tick()
{
    m_previousTick = m_currentTick;
    m_currentTick = Time::GetCurrentTick();
    ASSERT(m_currentTick >= m_previousTick);
    return GetDeltaSeconds();
}

void Platform::Timer::Reset()
{
    m_currentTick = m_previousTick = Time::GetCurrentTick();
}

Platform::TimeSlice Platform::Timer::GetTimeSlice() const
{
    return TimeSlice{m_previousTick, m_currentTick};
}

float Platform::Timer::GetDeltaSeconds() const
{
    return Time::ConvertTicksToSeconds(GetDeltaTicks());
}

float Platform::Timer::GetElapsedSeconds() const
{
    return Time::ConvertTicksToSeconds(GetElapsedTicks());
}

u64 Platform::Timer::GetDeltaTicks() const
{
    return m_currentTick - m_previousTick;
}

u64 Platform::Timer::GetElapsedTicks() const
{
    return Time::GetCurrentTick() - m_currentTick;
}
