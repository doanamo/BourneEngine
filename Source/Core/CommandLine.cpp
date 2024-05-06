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

    m_argc = argc;
    m_argv = argv;
    return true;
}

bool CommandLine::GetParam(const char* name, const char** value) const
{
    ASSERT(name != nullptr);

    // Parse parameter in '-name value` format. Parameters can start
    // with '-' or '--'. Does not support '-name=value' for simplicity,
    // so parameter name must be separated from value with a space.
    for(int i = 1; i < m_argc; ++i)
    {
        const char* arg = m_argv[i];
        if(arg[0] != '-')
        {
            continue;
        }

        const int nameOffset = arg[1] == '-' ? 2 : 1;
        const char* nameBegin = arg + nameOffset;
        if(strcmp(nameBegin, name) == 0)
        {
            if(value && i + 1 < m_argc)
            {
                *value = m_argv[i + 1];
            }

            return true;
        }
    }

    return false;
}

const char* CommandLine::GetExecutable() const
{
    ASSERT(m_argv);
    return m_argv[0];
}
