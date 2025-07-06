#include "Shared.hpp"
#include "Engine.hpp"
#include "Application.hpp"
#include "Platform/CommandLine.hpp"

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
    }
    else if(Logger::GetWarningCount() > 0)
    {
        LOG_WARNING("Exiting with %u warning(s) and %u error(s)",
            Logger::GetWarningCount(), Logger::GetErrorCount());
    }
#endif

    // #todo: Look for a way to override exit code if static destruction finds memory leaks.
}

// #todo: All declaration/definition comments should be using JetBrains format and be prettified by IDE.
int main(const int argc, const char* const* argv)
{
    // Setup process exit routine.
    std::atexit(OnProcessExit);

    // Create the application instance.
    UniquePtr<Application> application = Application::Create();
    ASSERT(application);

    Config config = application->GetConfig();
    LOG_MINIMUM_SEVERITY_SCOPE(config.common.logger.minimumSeverity);

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

    ExitCode exitCode = engine.Run(*application);
    return static_cast<int>(exitCode);
}
