#pragma once

namespace Platform
{
    class CommandLine final
    {
        struct Argument
        {
            Optional<StringView> name;
            Optional<StringView> value;
        };

        Array<Argument> m_arguments;

    public:
        static CommandLine& Get();

        void Parse(u32 argc, const char* const* argv);
        void Print() const;

        bool HasArgument(const StringView& argumentName) const;
        Optional<StringView> GetArgumentValue(const StringView& argumentName) const;
    };
}
