#include "Core/Shared.hpp"
#include "Core/Core.hpp"
#include "Common/Version.hpp"

void Core::Setup()
{
    Common::Setup();
    Memory::Setup();
    Logger::Setup();

    LOG("Engine version: %s (%s-%s-%s)", Version::Readable,
        Version::ChangeNumber, Version::BranchName, Version::CommitHash);
    LOG("Commit date: %s", Version::CommitDate);
}
