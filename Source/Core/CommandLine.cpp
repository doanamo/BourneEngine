#include "Core/Shared.hpp"
#include "Core/CommandLine.hpp"

CommandLine& CommandLine::Get()
{
    static CommandLine instance;
    return instance;
}

bool CommandLine::Setup(int argc, char** argv)
{
    ASSERT(argc > 0);
    ASSERT(argv != nullptr);

    return true;
}

bool CommandLine::Param(const char* name, const char** value) const
{
    return false;
}
