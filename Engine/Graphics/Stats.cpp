#include "Shared.hpp"
#include "Stats.hpp"

static Graphics::Stats g_graphicsStats;

Graphics::Stats& Graphics::Stats::Get()
{
    return g_graphicsStats;
}

void Graphics::Stats::AddFrameTimeSlice(const Platform::TimeSlice& timeSlice)
{
    m_frameTimeSamples[m_frameTimeRotationIndex] = timeSlice;

    m_updateTimer -= timeSlice.GetDurationSeconds();
    if(m_updateTimer <= 0.0f)
    {
        m_frameTimeMinimum = FLT_MAX;
        m_frameTimeMaximum = 0.0f;

        Platform::TimeSlice averageRange = Platform::TimeSlice::FromSecondsDuration(-1.0f, timeSlice.GetEndTicks());
        float totalFrameSampleDurations = 0.0f;
        float totalFrameSampleOverlaps = 0.0f;

        for(u32 i = 0; i < FrameTimeSamplesMax; i++)
        {
            if(!m_frameTimeSamples[i].IsValid())
                continue;

            float frameDuration = m_frameTimeSamples[i].GetDurationSeconds();
            totalFrameSampleDurations += frameDuration;

            float frameOverlap = m_frameTimeSamples[i].CalculateOverlap(averageRange);
            totalFrameSampleOverlaps += frameOverlap;

            // Calculate frame times based on whole frame slices within last second.
            // Always include at least one last frame in min/max calculations, even if partial.
            if(i == m_frameTimeRotationIndex || NearlyEqual(frameOverlap, 1.0f, 0.001f))
            {
                m_frameTimeMinimum = std::min(m_frameTimeMinimum, frameDuration);
                m_frameTimeMaximum = std::max(m_frameTimeMaximum, frameDuration);
            }

            // Prune stale frame slices so they won't be processed again.
            if(NearlyEqual(frameOverlap, 0.0f, 0.001f))
            {
                m_frameTimeSamples[i] = Platform::TimeSlice();
            }
        }

        // Extrapolate collected stats to full average frame if incomplete.
        // This can happen when framerate is much higher than sampling resolution.
        float frameSamplingCoverage = 1.0f / totalFrameSampleDurations;
        m_frameTimeAverage = 1.0f / (totalFrameSampleOverlaps * frameSamplingCoverage);
        m_framesPerSecond = totalFrameSampleOverlaps * frameSamplingCoverage;

        m_updateTimer = UpdateInterval;
        m_hasUpdated = true;
    }
    else
    {
        m_hasUpdated = false;
    }

    m_frameTimeRotationIndex = (m_frameTimeRotationIndex + 1) % FrameTimeSamplesMax;
}
