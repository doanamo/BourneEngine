#include "Shared.hpp"
#include "Stats.hpp"

Graphics::Stats& Graphics::Stats::Get()
{
    static Stats instance;
    return instance;
}

void Graphics::Stats::OnBeginFrame(float deltaTime)
{
    m_frameTimeSamples[m_frameTimeSampleIndex] = deltaTime;
    m_frameTimeSampleIndex = (m_frameTimeSampleIndex + 1) % FrameTimeSamplesMax;

    m_updateTimer -= deltaTime;
    if(m_updateTimer <= 0.0f)
    {
        m_frameTimeAverage = 0.0f;
        m_frameTimeMinimum = FLT_MAX;
        m_frameTimeMaximum = 0.0f;

        u32 validSampleCount = 0;
        for(u32 i = 0; i < FrameTimeSamplesMax; i++)
        {
            if(m_frameTimeSamples[i] > 0.0f)
            {
                m_frameTimeAverage += m_frameTimeSamples[i];
                ++validSampleCount;

                m_frameTimeMinimum = std::min(m_frameTimeMinimum, m_frameTimeSamples[i]);
                m_frameTimeMaximum = std::max(m_frameTimeMaximum, m_frameTimeSamples[i]);
            }
        }

        m_frameTimeAverage /= validSampleCount;
        m_framesPerSecond = 1.0f / m_frameTimeAverage;

        m_updateTimer = UpdateInterval;
        m_hasUpdated = true;
    }
    else
    {
        m_hasUpdated = false;
    }
}
