#include "Shared.hpp"
#include "Stats.hpp"
#include <limits>

Graphics::Stats Graphics::Stats::s_instance;

void Graphics::Stats::OnEndFrame()
{
    m_frameTimeSamples[m_frameTimeIndex] = m_timer.Tick();

    m_frameTimeMinimum = std::numeric_limits<f32>::max();
    m_frameTimeMaximum = 0.0f;

    f64 frameSamplesAccumulated = 0.0f;
    u32 frameSamplesAccounted = 0;

    for (u32 i = 0; i < ArraySize(m_frameTimeSamples); ++i)
    {
        // Do not accumulate samples that are too old, so
        // stats recover quicker in case of performance drops.
        if(frameSamplesAccumulated >= 1.0f)
            break;

        u32 index = (m_frameTimeIndex - i + ArraySize(m_frameTimeSamples)) % ArraySize(m_frameTimeSamples);
        f32 sample = m_frameTimeSamples[index];
        if(sample == 0.0f)
            break;

        m_frameTimeMinimum = std::min(m_frameTimeMinimum, sample);
        m_frameTimeMaximum = std::max(m_frameTimeMaximum, sample);

        frameSamplesAccumulated += sample;
        frameSamplesAccounted += 1;
    }

    m_frameTimeAverage = frameSamplesAccumulated / frameSamplesAccounted;
    m_framesPerSecond = 1.0f / m_frameTimeAverage;

    m_frameTimeIndex = (m_frameTimeIndex + 1) % ArraySize(m_frameTimeSamples);
}
