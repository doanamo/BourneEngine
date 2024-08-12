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

float Platform::ConvertTicksToSeconds(u64 ticks)
{
    return (float)ticks / g_timerTickFrequency.GetValue();
}

u64 Platform::ConvertSecondsToTicks(float seconds)
{
    return (u64)(seconds * g_timerTickFrequency.GetValue());
}

Platform::Timer::Timer()
{
    Reset();
}

Platform::Timer::~Timer() = default;

void Platform::Timer::Tick()
{
    m_previousTicks = m_currentTicks;
    m_currentTicks = ReadTicks();
    ASSERT(m_currentTicks >= m_previousTicks);
}

void Platform::Timer::Reset()
{
    m_currentTicks = m_previousTicks = ReadTicks();
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

u64 Platform::Timer::ReadTicks()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return counter.QuadPart;
}
