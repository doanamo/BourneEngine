#pragma once

class CommandLine final
{
public:
    static CommandLine& Get();

    bool Setup(int argc, char** argv);
    bool Param(const char* name, const char** value = nullptr) const;

private:

};
