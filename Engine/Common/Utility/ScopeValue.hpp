#pragma once

#include "NonCopyable.hpp"
#include "Optional.hpp"

template<typename Type>
class ScopeValue final : NonCopyable
{
    Type& m_variable;
    Type m_previous;

public:
    ScopeValue(Type& variable, Type value)
        : m_variable(variable)
        , m_previous(m_variable)
    {
        m_variable = value;
    }

    ~ScopeValue()
    {
        m_variable = m_previous;
    }
};
