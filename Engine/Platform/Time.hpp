#pragma once

namespace Time
{
    u64 GetTickFrequency();
    u64 GetCurrentTick();

    u64 ConvertSecondsToTicks(f32 seconds);
    f32 ConvertTicksToSeconds(u64 ticks);

    class Span final
    {
        u64 m_beginTick = 0;
        u64 m_endTick = 0;

    public:
        static Span FromDurationSeconds(u64 beginTick, f32 durationSeconds);

        Span() = default;
        Span(u64 beginTick, u64 endTick);

        f32 CalculateOverlapSeconds(const Span& slice) const;

        u64 GetDurationTicks() const;
        f32 GetDurationSeconds() const;

        u64 GetBeginTick() const
        {
            return m_beginTick;
        }

        u64 GetEndTick() const
        {
            return m_endTick;
        }

        bool IsValid() const
        {
            return m_endTick != 0 && m_beginTick <= m_endTick;
        }
    };

    class Timer final
    {
        u64 m_previousTick = 0;
        u64 m_currentTick = 0;

    public:
        Timer();

        f32 Tick();
        void Reset();

        Span GetTimeSlice() const;

        f32 GetDeltaSeconds() const;
        f32 GetElapsedSeconds() const;

        u64 GetDeltaTicks() const;
        u64 GetElapsedTicks() const;
    };

    class IntervalTimer final
    {
        Timer m_timer;
        f32 m_intervalSeconds = 0.0f;
        f32 m_remainingSeconds = 0.0f;

    public:
        IntervalTimer(f32 intervalSeconds, f32 delay = 0.0f);

        bool Tick();

        Timer& GetTimer()
        {
            return m_timer;
        }
    };
}
