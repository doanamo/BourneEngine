#include "Core/Shared.hpp"
#include "Core/Core.hpp"
#include "Core/CommandLine.hpp"
#include "Common/Version.hpp"

static bool ParseCommandLine()
{
    CommandLine& commandLine = CommandLine::Get();
    if(!commandLine.Setup(__argc, __argv))
    {
        LOG_ERROR("Failed to parse command line arguments");
        return false;
    }

    return true;
}

static void PrintVersion()
{
    LOG("Engine version: %s (%s-%s-%s)", Version::Readable,
        Version::ChangeNumber, Version::BranchName, Version::CommitHash);
    LOG("Commit date: %s", Version::CommitDate);
}

bool Core::Setup()
{
    if(!Common::Setup())
        return false;

    if(!Memory::Setup())
        return false;

    if(!Logger::Setup())
        return false;

    if(!Math::Setup())
        return false;

    if(!Structs::Setup())
        return false;

    PrintVersion();

    if(!ParseCommandLine())
        return false;

    return true;
}
