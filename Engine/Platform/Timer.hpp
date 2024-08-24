#pragma once

namespace Platform
{
    float ConvertTicksToSeconds(u64 ticks);
    u64 ConvertSecondsToTicks(float seconds);

    class Timer final
    {
    private:
    #ifdef PLATFORM_WINDOWS
        u64 m_currentTicks = 0;
        u64 m_previousTicks = 0;

        static u64 ReadTicks();
    #endif

    public:
        Timer();
        ~Timer();

        float Tick();
        void Reset();

        float GetDeltaSeconds() const;
        float GetElapsedSeconds() const;

        u64 GetDeltaTicks() const;
        u64 GetElapsedTicks() const;
    };
}
