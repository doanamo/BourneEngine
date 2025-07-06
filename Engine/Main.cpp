#include "Shared.hpp"
#include "Engine.hpp"
#include "Application.hpp"
#include "Platform/CommandLine.hpp"

ExitCodes g_exitCode = ExitCodes::Success;

void OnProcessExit()
{
#if ENABLE_MEMORY_STATS
    Memory::Stats::Get().LogMemoryLeaks();
#endif

#if ENABLE_LOGGER
    if(Logger::GetErrorCount() > 0)
    {
        LOG_ERROR("Exiting with %u warning(s) and %u error(s)",
            Logger::GetWarningCount(), Logger::GetErrorCount());

        if(g_exitCode == ExitCodes::Success)
        {
            g_exitCode = ExitCodes::LoggedErrors;
        }
    }
    else if(Logger::GetWarningCount() > 0)
    {
        LOG_WARNING("Exiting with %u warning(s) and %u error(s)",
            Logger::GetWarningCount(), Logger::GetErrorCount());

        if(g_exitCode == ExitCodes::Success)
        {
            g_exitCode = ExitCodes::LoggedWarnings;
        }
    }
#endif

    LOG_INFO("Process exit code: %u (%s)", static_cast<int>(g_exitCode), ExitCodeToString(g_exitCode));
    std::exit(static_cast<int>(g_exitCode));
}

int main(const int argc, const char* const* argv)
{
    // Setup process exit routine.
    std::atexit(OnProcessExit);

    // Create the application instance.
    UniquePtr<Application> application = Application::Create();
    ASSERT(application);

    Config config = application->GetConfig();

#if ENABLE_LOGGER
    Logger::g_minimumSeverity = config.logger.minimumSeverity;
#endif

    // Log initial engine info.
    LOG("Project: %s", BuildInfo::ProjectName);
    LOG("Application name: %s", Application::GetName());
    LOG("Application version: %s", ApplicationVersion::Readable);
    LOG("Engine version: %s", EngineVersion::Readable);
    LOG("Build configuration: %s", CONFIG_NAME);
    LOG("Build change: %s-%s-%s (%s)", BuildInfo::ChangeNumber,
        BuildInfo::BranchName, BuildInfo::CommitHash, BuildInfo::CommitDate);
    LOG("Project source path: %s", BuildInfo::ProjectSourcePath);
    LOG("Engine source path: %s", BuildInfo::EngineSourcePath);
    LOG("Platform: %s", PLATFORM_NAME);

    // Parse command line arguments.
    HeapString commandLineRaw;
    for(int i = 0; i < argc; i++)
    {
        commandLineRaw += " ";
        commandLineRaw += argv[i];
    }

    LOG_INFO("Process command line arguments:%s", *commandLineRaw);

    auto& commandLine = Platform::CommandLine::Get();
    commandLine.Parse(argc, argv);
    commandLine.Print();

    // Setup engine and run the application.
    Engine engine;
    SCOPE_GUARD
    {
        // Application must be destroyed before the engine.
        application.Reset();
    };

    if(!engine.Setup(config))
    {
        LOG_FATAL("Failed to setup engine");
        return -1;
    }

    if(!application->OnSetup())
    {
        LOG_FATAL("Failed to setup application");
        return -1;
    }

    g_exitCode = engine.Run(*application);
}
