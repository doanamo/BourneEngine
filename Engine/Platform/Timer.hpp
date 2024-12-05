#pragma once

namespace Platform
{
    class TimeSlice final
    {
    private:
        u64 m_beginTick = 0;
        u64 m_endTick = 0;

    public:
        static TimeSlice FromSecondsDuration(float durationSeconds, u64 startTicks);

        TimeSlice() = default;
        TimeSlice(u64 beginTick, u64 endTick);

        float CalculateOverlap(const TimeSlice& range) const;

        float GetSeconds() const;
        u64 GetTicks() const;

        u64 GetBeginTick() const
        {
            return m_beginTick;
        }

        u64 GetEndTicks() const
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
    private:
        u64 m_previousTicks = 0;
        u64 m_currentTicks = 0;

    public:
        static u64 ReadTicks();
        static float ConvertTicksToSeconds(u64 ticks);
        static u64 ConvertSecondsToTicks(float seconds);

        Timer();
        ~Timer();

        Platform::TimeSlice Tick();
        void Reset();

        TimeSlice GetTimeSlice();

        float GetDeltaSeconds() const;
        float GetElapsedSeconds() const;

        u64 GetDeltaTicks() const;
        u64 GetElapsedTicks() const;
    };
}
