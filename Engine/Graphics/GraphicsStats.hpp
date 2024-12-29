#pragma once

#include "Platform/Time.hpp"

namespace Graphics
{
    class Stats final
    {
        static Stats s_instance;

        Time::Timer m_timer;
        static constexpr u32 FrameTimeSamplesMax = 120;
        Time::TimeSlice m_frameTimeSamples[FrameTimeSamplesMax];
        u32 m_frameTimeRotationIndex = 0;

        static constexpr float UpdateInterval = 0.2f;
        float m_updateTimer = UpdateInterval;
        bool m_hasUpdated = false;

        float m_framesPerSecond = 0.0f;
        float m_frameTimeAverage = 0.0f;
        float m_frameTimeMinimum = 0.0f;
        float m_frameTimeMaximum = 0.0f;

        Stats() = default;

    public:
        static Stats& Get()
        {
            return s_instance;
        }

        void OnEndFrame();

        bool HasUpdated() const
        {
            return m_hasUpdated;
        }

        float GetFramesPerSecond() const
        {
            return m_framesPerSecond;
        }

        float GetFrameTimeAverage() const
        {
            return m_frameTimeAverage;
        }

        float GetFrameTimeMinimum() const
        {
            return m_frameTimeMinimum;
        }

        float GetFrameTimeMaximum() const
        {
            return m_frameTimeMaximum;
        }

    private:
        void AddFrameTime(const Time::TimeSlice& timeSlice);
    };
}
