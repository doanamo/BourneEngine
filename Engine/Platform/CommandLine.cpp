#include "Shared.hpp"
#include "CommandLine.hpp"

Platform::CommandLine& Platform::CommandLine::Get()
{
    static CommandLine instance;
    return instance;
}

void Platform::CommandLine::Parse(const u32 argc, const char* const* argv)
{
    ASSERT(argc > 0, "Command line argument count cannot be zero");
    ASSERT(argv, "Command line arguments cannot be nullptr");

    m_arguments.Clear();
    m_arguments.Reserve(argc);
    for(int i = 0; i < argc; ++i)
    {
        ASSERT(argv[i] != nullptr);
        StringView argument = argv[i];
        if(argument.StartsWith("-"))
        {
            argument.RemoveLeft(1);
            if(argument.StartsWith("-"))
            {
                argument.RemoveLeft(1);
            }

            if(Optional<u64> index = argument.Find("="))
            {
                m_arguments.Add(Argument
                {
                    .name = argument.SubStringLeftAt(index.GetValue()),
                    .value = argument.SubStringRightAt(index.GetValue() + 1)
                });
            }
            else
            {
                m_arguments.Add(Argument
                {
                    .name = argument,
                    .value = ""
                });
            }
        }
        else
        {
            m_arguments.Add(Argument
            {
                .name = "",
                .value = argv[i]
            });
        }
    }
}

void Platform::CommandLine::Print() const
{
    LOG("Command line arguments:");

    u64 index = 0;
    for(const Argument& argument : m_arguments)
    {
        if(argument.name.IsEmpty())
        {
            LOG("  %u: %.*s", index,
                argument.value.GetLength(), argument.value.GetData());
        }
        else if(argument.value.IsEmpty())
        {
            LOG("  %u: -%.*s", index,
                argument.name.GetLength(), argument.name.GetData());
        }
        else
        {
            LOG("  %u: -%.*s=\"%.*s\"", index,
                argument.name.GetLength(), argument.name.GetData(),
                argument.value.GetLength(), argument.value.GetData());
        }

        index++;
    }
}
