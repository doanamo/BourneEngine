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
                StringView name = argument.SubStringLeftAt(index.GetValue());
                StringView value = argument.SubStringRightAt(index.GetValue() + 1);
                if(value.StartsWith("\"") && value.EndsWith("\""))
                {
                    value.RemoveLeft(1);
                    value.RemoveRight(1);
                }

                m_arguments.Add(Argument
                {
                    .name = name,
                    .value = value,
                });
            }
            else
            {
                m_arguments.Add(Argument
                {
                    .name = argument,
                    .value = {},
                });
            }
        }
        else
        {
            m_arguments.Add(Argument
            {
                .name = {},
                .value = StringView(argv[i]),
            });
        }
    }
}

void Platform::CommandLine::Print() const
{
    LOG("Parsed command line arguments:");

    u64 index = 0;
    for(const auto& [name, value] : m_arguments)
    {
        LOG_NO_SOURCE_LINE_SCOPE();

        if(!name.HasValue())
        {
            LOG("  %u: " STRING_VIEW_FORMAT, index, STRING_VIEW_VARG(value.GetValue()));
        }
        else if(!value.HasValue())
        {
            LOG("  %u: -" STRING_VIEW_FORMAT, index, STRING_VIEW_VARG(value.GetValue()));
        }
        else
        {
            LOG("  %u: -" STRING_VIEW_FORMAT "=\"" STRING_VIEW_FORMAT "\"", index,
                STRING_VIEW_VARG(name.GetValue()), STRING_VIEW_VARG(value.GetValue()));
        }

        index++;
    }
}

bool Platform::CommandLine::HasArgument(const StringView& argumentName) const
{
    return m_arguments.ContainsPredicate([&argumentName](const Argument& argument)
    {
        return argument.name ? *argument.name == argumentName : false;
    });
}

Optional<StringView> Platform::CommandLine::GetArgumentValue(const StringView& argumentName) const
{
    const Argument* argument = m_arguments.FindPredicate([&argumentName](const Argument& argument)
    {
        return argument.name ? *argument.name == argumentName : false;
    });

    return argument ? argument->value : Optional<StringView>();
}
