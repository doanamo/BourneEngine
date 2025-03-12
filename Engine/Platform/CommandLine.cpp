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
    for(const auto& [name, value] : m_arguments)
    {
        LOG_NO_SOURCE_LINE_SCOPE();

        if(name.IsEmpty())
        {
            LOG("  %u: %.*s", index,
                value.GetLength(), value.GetData());
        }
        else if(value.IsEmpty())
        {
            LOG("  %u: -%.*s", index,
                name.GetLength(), name.GetData());
        }
        else
        {
            LOG("  %u: -%.*s=\"%.*s\"", index,
                name.GetLength(), name.GetData(),
                value.GetLength(), value.GetData());
        }

        index++;
    }
}

bool Platform::CommandLine::HasArgument(const StringView& name) const
{
    return m_arguments.ContainsPredicate([&name](const Argument& argument)
    {
        return argument.name == name;
    });
}
