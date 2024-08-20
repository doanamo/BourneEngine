#include "Shared.hpp"
#include "CommandLine.hpp"

Platform::CommandLine& Platform::CommandLine::Get()
{
    static CommandLine instance;
    return instance;
}

bool Platform::CommandLine::Setup(u32 argc, char** argv)
{
    ASSERT(argc > 0);
    ASSERT(argv != nullptr);

    for(int i = 0; i < argc; ++i)
    {
        ASSERT(argv[i] != nullptr);
    }

    m_argumentCount = argc;
    m_argumentArray = argv;

    return true;
}

bool Platform::CommandLine::GetParameter(const char* name, const char** value) const
{
    ASSERT(name != nullptr);

    // Parse parameter in '-name value` format. Parameters can start
    // with '-' or '--'. Does not support '-name=value' for simplicity,
    // so parameter name must be separated from value with a space.
    for(int i = 1; i < m_argumentCount; ++i)
    {
        const char* argument = m_argumentArray[i];
        if(argument[0] != '-')
        {
            // Parameters must start with a slash
            continue;
        }

        const int nameOffset = argument[1] == '-' ? 2 : 1;
        const char* nameBegin = argument + nameOffset;
        if(strcmp(nameBegin, name) == 0)
        {
            GetParameterValue(i, value);
            return true;
        }
    }

    return false;
}

void Platform::CommandLine::GetParameterValue(u32 parameterIndex, const char** value) const
{
    ASSERT(parameterIndex > 0);
    ASSERT(parameterIndex < m_argumentCount);
    if(value == nullptr)
        return;

    const int nextArgumentIndex = parameterIndex + 1;
    if(nextArgumentIndex >= m_argumentCount)
    {
        // Parameter is the last argument
        *value = "";
        return;
    }

    const char* nextArgument = m_argumentArray[nextArgumentIndex];
    if(nextArgument[0] == '-')
    {
        // Next argument is a parameter
        *value = "";
        return;
    }

    *value = nextArgument;
}

const char* Platform::CommandLine::GetExecutable() const
{
    ASSERT(m_argumentCount > 0);
    ASSERT(m_argumentArray != nullptr);
    return m_argumentArray[0];
}
