#pragma once

class CommandLine final
{
public:
    static CommandLine& Get();

    bool Setup(u32 argc, char** argv);
    bool GetParam(const char* name, const char** value = nullptr) const;
    const char* GetExecutable() const;

private:
    u32 m_argc = 0;
    char** m_argv = nullptr;
};
