#pragma once

class CommandLine final
{
public:
    static CommandLine& Get();

    bool Parse(int argc, char* argv[]);
};
