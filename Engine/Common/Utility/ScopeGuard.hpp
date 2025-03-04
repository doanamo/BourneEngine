#pragma once

template<typename Type>
class ScopeGuard final : NonCopyable
{
    Type m_function;

public:
    explicit ScopeGuard(Type&& function)
        : m_function(function)
    {}

    ~ScopeGuard()
    {
        m_function();
    }
};

class ScopeGuardFactory
{
public:
    template<typename Type>
    ScopeGuard<Type> operator+(Type&& function)
    {
        return ScopeGuard<Type>(std::forward<Type>(function));
    }
};

#define SCOPE_GUARD_STRING(line) scopeGuardLine ## line
#define SCOPE_GUARD_NAME(line) SCOPE_GUARD_STRING(line)

#define SCOPE_GUARD_VARIABLE auto SCOPE_GUARD_NAME(__LINE__)
#define SCOPE_GUARD SCOPE_GUARD_VARIABLE = ScopeGuardFactory() + [&]()
