#pragma once

#include "Common/Utility/Singleton.hpp"

namespace Platform
{
    class CommandLine final : public Singleton<CommandLine>
    {
        struct Argument
        {
            Optional<StringView> name;
            Optional<StringView> value;
        };

        Array<Argument> m_arguments;

    public:
        void Parse(u32 argc, const char* const* argv);
        void Print() const;

        bool HasArgument(const StringView& argumentName) const;
        Optional<StringView> GetArgumentValue(const StringView& argumentName) const;
    };
}
