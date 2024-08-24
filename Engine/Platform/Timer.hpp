#pragma once

namespace Platform
{
    class TimeSlice final
    {
    private:
        u64 m_startTicks = 0;
        u64 m_endTicks = 0;

    public:
        static TimeSlice FromSecondsDuration(float secondsDuration, u64 ticks);

        TimeSlice() = default;
        TimeSlice(u64 startTicks, u64 endTicks);

        float CalculateOverlap(const TimeSlice& range) const;

        float GetDurationSeconds() const;
        u64 GetDurationTicks() const;

        u64 GetStartTicks() const
        {
            return m_startTicks;
        }

        u64 GetEndTicks() const
        {
            return m_endTicks;
        }

        bool IsValid() const
        {
            return m_endTicks != 0 && m_startTicks <= m_endTicks;
        }
    };

    class Timer final
    {
    private:
        u64 m_currentTicks = 0;
        u64 m_previousTicks = 0;

    public:
        static u64 ReadTicks();
        static float ConvertTicksToSeconds(u64 ticks);
        static u64 ConvertSecondsToTicks(float seconds);

        Timer();
        ~Timer();

        float Tick();
        void Reset();

        TimeSlice GetTimeSlice();

        float GetDeltaSeconds() const;
        float GetElapsedSeconds() const;

        u64 GetDeltaTicks() const;
        u64 GetElapsedTicks() const;
    };
}
