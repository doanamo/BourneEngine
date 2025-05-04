#pragma once

namespace Time
{
    u64 GetTickFrequency();
    u64 GetCurrentTick();

    u64 ConvertSecondsToTicks(float seconds);
    float ConvertTicksToSeconds(u64 ticks);

    // #todo: Rename from Time::TimeSlice to Time::Slice/Time::Span.
    class TimeSlice final
    {
        u64 m_beginTick = 0;
        u64 m_endTick = 0;

    public:
        static TimeSlice FromDurationSeconds(u64 beginTick, float durationSeconds);

        TimeSlice() = default;
        TimeSlice(u64 beginTick, u64 endTick);

        float CalculateOverlapSeconds(const TimeSlice& slice) const;

        u64 GetDurationTicks() const;
        float GetDurationSeconds() const;

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

        float Tick();
        void Reset();

        TimeSlice GetTimeSlice() const;

        float GetDeltaSeconds() const;
        float GetElapsedSeconds() const;

        u64 GetDeltaTicks() const;
        u64 GetElapsedTicks() const;
    };
}
