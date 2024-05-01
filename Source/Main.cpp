#include "Shared.hpp"
#include "Common/Version.hpp"

int main()
{
    Memory::Setup();
    Logger::Setup();

    LOG("Engine version: {} ({}-{}-{})", Version::Readable,
        Version::ChangeNumber, Version::BranchName, Version::CommitHash);
    LOG("Commit date: {}", Version::CommitDate);
    LOG("Build date: {}", Version::BuildDate);

    return 0;
}
