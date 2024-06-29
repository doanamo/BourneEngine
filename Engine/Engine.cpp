#include "Shared.hpp"
#include "Engine.hpp"
#include "Platform/CommandLine.hpp"

void ParseCommandLine()
{
    CommandLine& commandLine = CommandLine::Get();
    if(!commandLine.Setup(__argc, __argv))
    {
        LOG_FATAL("Failed to parse command line arguments");
    }
}

void Engine::Setup()
{
    ParseCommandLine();

    Logger::Setup();
    Memory::Setup();
    Platform::Setup();

    LOG("Build configuration: %s", CONFIG_NAME);
    LOG("Engine version: %s (%s-%s-%s)", Version::Readable,
        Version::ChangeNumber, Version::BranchName, Version::CommitHash);
    LOG("Engine commit date: %s", Version::CommitDate);
}
