#pragma once

namespace Platform
{
    class CommandLine final
    {
    private:
        struct Argument
        {
            StringView name;
            StringView value;
        };

        Array<Argument> m_arguments;

    public:
        static CommandLine& Get();

        void Setup(u32 argc, const char* const* argv);
        void Print() const;
    };
}
