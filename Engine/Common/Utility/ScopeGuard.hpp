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

#define SCOPE_GUARD auto UNIQUE_NAME(scopeGuard) = ScopeGuardFactory() + [&]()
