#pragma once

namespace Platform
{
    class CommandLine final
    {
        struct Argument
        {
            StringView name;
            StringView value;
        };

        Array<Argument> m_arguments;

    public:
        static CommandLine& Get();

        void Parse(u32 argc, const char* const* argv);
        void Print() const;

        bool HasArgument(const StringView& name) const;
    };
}
