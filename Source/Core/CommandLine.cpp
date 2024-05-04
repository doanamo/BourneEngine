#include "Core/Shared.hpp"
#include "Core/CommandLine.hpp"

CommandLine& CommandLine::Get()
{
    static CommandLine instance;
    return instance;
}

bool CommandLine::Parse(int argc, char* argv[])
{

    return true;
}
