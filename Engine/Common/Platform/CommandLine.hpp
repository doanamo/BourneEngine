#pragma once

class CommandLine final
{
public:
    static CommandLine& Get();

    bool Setup(u32 argc, char** argv);
    bool GetParameter(const char* name, const char** value = nullptr) const;
    const char* GetExecutable() const;

private:
    void GetParameterValue(u32 parameterIndex, const char** value) const;

private:
    u32 m_argumentCount = 0;
    char** m_argumentArray = nullptr;
};
