#include "Shared.hpp"
#include "Engine.hpp"
#include "Platform/CommandLine.hpp"
#include "Graphics/Stats.hpp"

Engine::~Engine()
{
    LOG_DEBUG("Destroying engine...");
}

bool Engine::Setup(const Config& config)
{
    ASSERT(!m_setupCalled && !m_setupSucceeded);
    LOG_DEBUG("Setting up engine...");

    if(!config.headless)
    {
        if(!m_window.Setup(config.window))
        {
            LOG_ERROR("Failed to setup platform window");
            return false;
        }
        else
        {
           auto windowTitle = InlineString<64>::Format("%s %s", Application::GetName(), EngineVersion::Readable);
            m_window.SetTitle(windowTitle);
        }

        if(!m_renderApi.Setup(&m_window, config.render))
        {
            LOG_ERROR("Failed to setup graphics render API");
            return false;
        }
    }

    LOG_SUCCESS("Engine setup complete");
    return m_setupSucceeded = true;
}

ExitCodes Engine::Run(Application& application)
{
    if(auto exitCode = application.OnRun())
        return exitCode.GetValue();

    LOG_INFO("Starting main loop...");
    m_window.Show();

    while(true)
    {
        float deltaTime = m_timer.Tick();

        m_window.ProcessEvents();
        if(m_window.IsClosing())
            break;

        application.OnUpdate(deltaTime);

        m_renderApi.BeginFrame();
        {
            application.OnDraw(1.0f);
        }
        m_renderApi.EndFrame();

#if !CONFIG_RELEASE
        Graphics::Stats& graphicsStats = Graphics::Stats::Get();
        Memory::Stats& memoryStats = Memory::Stats::Get();

        static Time::IntervalTimer titleUpdateTimer(0.2f);
        if(titleUpdateTimer.Tick())
        {
            auto titleStats = InlineString<128>::Format(
                " | %.0f FPS (min: %.2fms, avg: %.2fms, max: %.2fms) | Allocations: %llu (%llu bytes)",
                graphicsStats.GetFramesPerSecond(),
                graphicsStats.GetFrameTimeMinimum() * 1000.0f,
                graphicsStats.GetFrameTimeAverage() * 1000.0f,
                graphicsStats.GetFrameTimeMaximum() * 1000.0f,
                memoryStats.GetAllocatedTotalCount(),
                memoryStats.GetAllocatedTotalBytes());

            m_window.SetTitleSuffix(titleStats.GetData());
        }

        memoryStats.ResetTotalAllocations();
#endif
    }

    LOG_INFO("Exiting application...");
    return ExitCodes::Success;
}

Platform::Window& Engine::GetWindow()
{
    return m_window;
}

Graphics::RenderApi& Engine::GetRenderApi()
{
    return m_renderApi;
}
