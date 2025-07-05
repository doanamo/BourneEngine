#include "Shared.hpp"
#include "Engine.hpp"
#include "Platform/CommandLine.hpp"
#include "Graphics/Stats.hpp"

const char* g_applicationName = "";

Engine::Engine::~Engine()
{
    LOG_DEBUG("Destroying engine...");
}

bool Engine::Engine::Setup(const Config& config)
{
    ASSERT(!m_setupCalled && !m_setupSucceeded);
    LOG_DEBUG("Setting up engine...");

    g_applicationName = config.applicationName;

    LogPreSetupInfo();
    ParseCommandLine(config);

    if(!config.headless)
    {
        auto windowTitle = InlineString<64>::Format("%s %s", g_applicationName, EngineVersion::Readable);
        if(!m_window.Setup(windowTitle, config.platform.window.width, config.platform.window.height))
        {
            LOG_ERROR("Failed to setup platform window");
            return false;
        }

        if(!m_graphics.Setup(&m_window))
        {
            LOG_ERROR("Failed to setup graphics system");
            return false;
        }
    }

    LOG_SUCCESS("Engine setup complete");
    return m_setupSucceeded = true;
}

int Engine::Engine::Run()
{
    LOG_INFO("Starting main loop...");
    m_window.Show();

    while(true)
    {
        float deltaTime = m_timer.Tick();

        Platform::Window::ProcessEvents();
        if(m_window.IsClosing())
            break;

        m_graphics.BeginFrame();
        {
        }
        m_graphics.EndFrame();

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
    return 0;
}

void Engine::Engine::LogPreSetupInfo()
{
    LOG("Project: %s", BuildInfo::ProjectName);
    LOG("Application name: %s", g_applicationName);
    LOG("Application version: %s", ApplicationVersion::Readable);
    LOG("Engine version: %s", EngineVersion::Readable);
    LOG("Build configuration: %s", CONFIG_NAME);
    LOG("Build change: %s-%s-%s (%s)", BuildInfo::ChangeNumber,
        BuildInfo::BranchName, BuildInfo::CommitHash, BuildInfo::CommitDate);
    LOG("Project source path: %s", BuildInfo::ProjectSourcePath);
    LOG("Engine source path: %s", BuildInfo::EngineSourcePath);
    LOG("Platform: %s", PLATFORM_NAME);
}

void Engine::Engine::ParseCommandLine(const Config& config)
{
    HeapString commandLineRaw;
    for(int i = 0; i < config.commandLineArgc; i++)
    {
        commandLineRaw += " ";
        commandLineRaw += config.commandLineArgv[i];
    }

    LOG_INFO("Process command line arguments:%s", *commandLineRaw);

    auto& commandLine = Platform::CommandLine::Get();
    commandLine.Parse(config.commandLineArgc, config.commandLineArgv);
    commandLine.Print();
}

Platform::Window& Engine::Engine::GetWindow()
{
    return m_window;
}

Graphics::System& Engine::Engine::GetGraphics()
{
    return m_graphics;
}

const char* Engine::GetApplicationName()
{
    return g_applicationName;
}
