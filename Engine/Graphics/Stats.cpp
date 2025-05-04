#include "Shared.hpp"
#include "Stats.hpp"
#include <limits>

Graphics::Stats Graphics::Stats::s_instance;

Graphics::Stats::Stats()
{
    ResetFrameTimes();
}

void Graphics::Stats::OnEndFrame()
{
    AddFrameTime(m_timer.Tick());
}

void Graphics::Stats::ResetFrameTimes()
{
    m_frameTimeAccumulated = 0.0f;
    m_frameTimeSampleCount = 0;

    m_framesPerSecond = 0.0f;
    m_frameTimeAverage = 0.0f;
    m_frameTimeMinimum = std::numeric_limits<float>::max();
    m_frameTimeMaximum = 0.0f;
}

void Graphics::Stats::AddFrameTime(const float frameTime)
{
    m_frameTimeMinimum = std::min(m_frameTimeMinimum, frameTime);
    m_frameTimeMaximum = std::max(m_frameTimeMaximum, frameTime);

    m_updateTimer -= frameTime;
    if(m_updateTimer <= 0.0f)
    {
        m_updateTimer = UpdateInterval;
    }
}
