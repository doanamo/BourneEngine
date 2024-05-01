#include "Shared.hpp"
#include "Common/Version.hpp"

int main()
{
    Memory::Setup();
    Logger::Setup();

    LOG("Engine version: %s (%s-%s-%s)", Version::Readable,
        Version::ChangeNumber, Version::BranchName, Version::CommitHash);
    LOG("Commit date: %s", Version::CommitDate);
    LOG("Build date: %s", Version::BuildDate);

    return 0;
}
