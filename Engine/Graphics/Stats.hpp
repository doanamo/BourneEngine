#pragma once

#include "Common/Utility/Singleton.hpp"
#include "Platform/Time.hpp"

namespace Graphics
{
    class Stats final : public Singleton<Stats>
    {
        Time::Timer m_timer;
        f32 m_frameTimeSamples[240] = {};
        u32 m_frameTimeIndex = 0;

        f32 m_framesPerSecond = 0.0f;
        f32 m_frameTimeAverage = 0.0f;
        f32 m_frameTimeMinimum = 0.0f;
        f32 m_frameTimeMaximum = 0.0f;

    public:
        void OnEndFrame();

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
    };
}
