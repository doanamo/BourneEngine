#include "Core/Shared.hpp"
#include "Core/CommandLine.hpp"

CommandLine& CommandLine::Get()
{
    static CommandLine instance;
    return instance;
}

bool CommandLine::Setup(u32 argc, char** argv)
{
    ASSERT(argc > 0);
    ASSERT(argv != nullptr);

    m_argumentCount = argc;
    m_argumentArray = argv;
    return true;
}

bool CommandLine::GetParameter(const char* name, const char** value) const
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
            continue;
        }

        const int nameOffset = argument[1] == '-' ? 2 : 1;
        const char* nameBegin = argument + nameOffset;
        if(strcmp(nameBegin, name) == 0)
        {
            if(value && i + 1 < m_argumentCount)
            {
                *value = m_argumentArray[i + 1];
            }

            return true;
        }
    }

    return false;
}

const char* CommandLine::GetExecutable() const
{
    ASSERT(m_argumentCount > 0);
    ASSERT(m_argumentArray != nullptr);
    return m_argumentArray[0];
}
