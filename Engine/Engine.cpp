#include "Shared.hpp"
#include "Engine.hpp"
#include "Platform/CommandLine.hpp"

void ParseCommandLine()
{
    auto& commandLine = Platform::CommandLine::Get();
    if(!commandLine.Setup(__argc, __argv))
    {
        LOG_FATAL("Failed to parse command line arguments");
    }
}

void Engine::Setup()
{
    ParseCommandLine();

#if 0
    LOG_DEBUG("Example debug message");
    LOG_INFO("Example info message");
    LOG_SUCCESS("Example success message");
    LOG_WARNING("Example warning message");
    LOG_ERROR("Example error message");
    LOG_FATAL("Example fatal message");
#endif

    LOG("Build platform: %s", PLATFORM_NAME);
    LOG("Build configuration: %s", CONFIG_NAME);
    LOG("Engine version: %s (%s-%s-%s)", Version::Readable,
        Version::ChangeNumber, Version::BranchName, Version::CommitHash);
    LOG("Engine commit date: %s", Version::CommitDate);
}
