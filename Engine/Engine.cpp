#include "Precompiled.hpp"
#include "Engine.hpp"
#include "Common/Platform/CommandLine.hpp"

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

    LOG("Engine version: %s (%s-%s-%s)", Version::Readable,
        Version::ChangeNumber, Version::BranchName, Version::CommitHash);
    LOG("Commit date: %s", Version::CommitDate);
}
