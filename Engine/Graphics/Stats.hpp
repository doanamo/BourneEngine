#pragma once

namespace Graphics
{
    class Stats final
    {
    private:
        static constexpr u32 FrameTimeSamplesMax = 120;
        float m_frameTimeSamples[FrameTimeSamplesMax] = {};
        u32 m_frameTimeSampleIndex = 0;

        static constexpr float UpdateInterval = 0.2f;
        float m_updateTimer = UpdateInterval;
        bool m_hasUpdated = false;

        float m_framesPerSecond = 0.0f;
        float m_frameTimeAverage = 0.0f;
        float m_frameTimeMinimum = 0.0f;
        float m_frameTimeMaximum = 0.0f;

    public:
        static Stats& Get();

        Stats() = default;
        ~Stats() = default;

        void OnBeginFrame(float deltaTime);
        void OnEndFrame();

        float HasUpdated() const
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
    };
}
