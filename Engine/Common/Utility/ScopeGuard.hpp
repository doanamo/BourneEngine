#pragma once

template<typename Type>
class ScopeGuard : NonCopyable
{
public:
    ScopeGuard(Type function)
        : m_function(function)
    {}

    ScopeGuard(ScopeGuard<Type>&& other) noexcept
        : m_function(std::move(other.m_function))
    {}

    ScopeGuard<Type>& operator=(ScopeGuard<Type>&& other) noexcept
    {
        ASSERT_SLOW(this != &other);
        m_function = std::move(other.m_function);
        return *this;
    }

    ~ScopeGuard()
    {
        m_function();
    }

private:
    Type m_function;
};

template<typename Type>
ScopeGuard<Type> MakeScopeGuard(Type function)
{
    return ScopeGuard<Type>(function);
}

#define SCOPE_GUARD_STRING(line) scopeGuardLine ## line
#define SCOPE_GUARD_NAME(line) SCOPE_GUARD_STRING(line)

#define SCOPE_GUARD_VARIABLE auto SCOPE_GUARD_NAME(__LINE__)
#define SCOPE_GUARD SCOPE_GUARD_VARIABLE = MakeScopeGuard
