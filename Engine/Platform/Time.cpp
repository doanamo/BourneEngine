#include "Shared.hpp"
#include "Platform/Time.hpp"

u64 Time::ConvertSecondsToTicks(const f32 seconds)
{
    ASSERT(seconds >= 0.0f);
    const u64 tickFrequency = GetTickFrequency();
    const f64 wholeSeconds = std::trunc(seconds);
    const u64 wholeSecondsTicks = static_cast<u64>(wholeSeconds) * tickFrequency;
    const u64 partialSecondTicks = static_cast<u64>((seconds - wholeSeconds) * static_cast<f64>(tickFrequency));
    return wholeSecondsTicks + partialSecondTicks;
}

float Time::ConvertTicksToSeconds(const u64 ticks)
{
    const u64 tickFrequency = GetTickFrequency();
    const u64 wholeSecondsTicks = (ticks / tickFrequency) * tickFrequency;
    const u64 partialSecondTicks = ticks - wholeSecondsTicks;
    const f64 wholeSeconds = static_cast<f64>(wholeSecondsTicks) / static_cast<f64>(tickFrequency);
    const f64 partialSecond = static_cast<f64>(partialSecondTicks) / static_cast<f64>(tickFrequency);
    return static_cast<f32>(wholeSeconds + partialSecond);
}

Time::Span Time::Span::FromDurationSeconds(const u64 beginTick, const f32 durationSeconds)
{
    const u64 durationTicks = ConvertSecondsToTicks(std::abs(durationSeconds));
    if(durationSeconds >= 0.0f)
    {
        return {beginTick, beginTick + durationTicks};
    }
    else
    {
        return {beginTick - durationTicks, beginTick};
    }
}

Time::Span::Span(const u64 beginTick, const u64 endTick)
    : m_beginTick(beginTick)
    , m_endTick(endTick)
{
    ASSERT(m_beginTick <= m_endTick);
}

float Time::Span::CalculateOverlapSeconds(const Span& other) const
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

    const f64 overlapRatio = static_cast<f64>(overlapTicks) / static_cast<f64>(GetDurationTicks());
    ASSERT(overlapRatio >= 0.0f && overlapRatio <= 1.0f);
    return static_cast<f32>(overlapRatio);
}

u64 Time::Span::GetDurationTicks() const
{
    return m_endTick - m_beginTick;
}

f32 Time::Span::GetDurationSeconds() const
{
    return ConvertTicksToSeconds(GetDurationTicks());
}

Time::Timer::Timer()
{
    Reset();
}

f32 Time::Timer::Tick()
{
    m_previousTick = m_currentTick;
    m_currentTick = GetCurrentTick();
    ASSERT(m_currentTick >= m_previousTick);
    return GetDeltaSeconds();
}

void Time::Timer::Reset()
{
    m_currentTick = m_previousTick = GetCurrentTick();
}

Time::Span Time::Timer::GetTimeSlice() const
{
    return Span{m_previousTick, m_currentTick};
}

f32 Time::Timer::GetDeltaSeconds() const
{
    return ConvertTicksToSeconds(GetDeltaTicks());
}

f32 Time::Timer::GetElapsedSeconds() const
{
    return ConvertTicksToSeconds(GetElapsedTicks());
}

u64 Time::Timer::GetDeltaTicks() const
{
    return m_currentTick - m_previousTick;
}

u64 Time::Timer::GetElapsedTicks() const
{
    return GetCurrentTick() - m_currentTick;
}

Time::IntervalTimer::IntervalTimer(f32 intervalSeconds, f32 delaySeconds)
    : m_intervalSeconds(intervalSeconds)
    , m_remainingSeconds(delaySeconds)
{
    ASSERT(m_intervalSeconds >= 0.0f);
}

bool Time::IntervalTimer::Tick()
{
    m_remainingSeconds -= m_timer.Tick();
    if(m_remainingSeconds <= 0.0f)
    {
        m_remainingSeconds = std::max(m_remainingSeconds, -m_intervalSeconds);
        m_remainingSeconds += m_intervalSeconds;
        return true;
    }

    return false;
}
