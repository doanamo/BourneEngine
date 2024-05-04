#include "Core/Shared.hpp"
#include "Core/Core.hpp"
#include "Core/CommandLine.hpp"
#include "Common/Version.hpp"

static void ParseCommandLine()
{
    CommandLine& commandLine = CommandLine::Get();
    commandLine.Parse(__argc, __argv);
}

static void PrintVersion()
{
    LOG("Engine version: %s (%s-%s-%s)", Version::Readable,
        Version::ChangeNumber, Version::BranchName, Version::CommitHash);
    LOG("Commit date: %s", Version::CommitDate);
}

void Core::Setup()
{
    Common::Setup();
    Memory::Setup();
    Logger::Setup();

    ParseCommandLine();
    PrintVersion();
}
