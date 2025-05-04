#pragma once

#include "Platform/Time.hpp"

namespace Graphics
{
    class Stats final
    {
        static Stats s_instance;

        static constexpr float UpdateInterval = 1.0f;
        float m_updateTimer = UpdateInterval;
        bool m_hasUpdated = false;

        Time::Timer m_timer;
        float m_frameTimeAccumulated = 0.0f;
        int m_frameTimeSampleCount = 0;

        float m_framesPerSecond = 0.0f;
        float m_frameTimeAverage = 0.0f;
        float m_frameTimeMinimum = std::numeric_limits<float>::max();
        float m_frameTimeMaximum = 0.0f;

        Stats();

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
        void ResetFrameTimes();
        void AddFrameTime(float frameTime);
    };
}
