#include "Shared.hpp"
#include "Engine.hpp"
#include "Platform/CommandLine.hpp"

const char* g_applicationName = "";

void Engine::Setup(const Config& config)
{
    ASSERT(config.applicationName, "Application name must be specified");
    g_applicationName = config.applicationName;

    LOG("Build version: %s (%s-%s-%s)",
        BuildVersion::Readable, BuildVersion::ChangeNumber,
        BuildVersion::BranchName, BuildVersion::CommitHash);
    LOG("Build commit date: %s", BuildVersion::CommitDate);
    LOG("Build configuration: %s", CONFIG_NAME);

    auto& commandLine = Platform::CommandLine::Get();
    commandLine.Parse(config.commandLineArgumentCount, config.commandLineArguments);
    commandLine.Print();

#if 0
    LOG_DEBUG("Example debug message");
    LOG_INFO("Example info message");
    LOG_SUCCESS("Example success message");
    LOG_WARNING("Example warning message");
    LOG_ERROR("Example error message");
    LOG_FATAL("Example fatal message");
#endif
}

const char* Engine::GetApplicationName()
{
    return g_applicationName;
}
