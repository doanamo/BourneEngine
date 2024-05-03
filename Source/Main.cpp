#include "Shared.hpp"
#include "Common/Version.hpp"

int main()
{
    Common::Setup();
    Memory::Setup();
    Logger::Setup();

    LOG("Engine version: %s (%s-%s-%s)", Version::Readable,
        Version::ChangeNumber, Version::BranchName, Version::CommitHash);
    LOG("Commit date: %s", Version::CommitDate);

    return 0;
}
