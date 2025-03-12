#pragma once

template<typename Type>
class Optional final
{
    Type m_value;
    bool m_hasValue = false;

public:
    Optional() = default;
    Optional(const Type& value)
        : m_value(value)
        , m_hasValue(true)
    {}

    explicit Optional(Type&& value)
        : m_value(std::move(value))
        , m_hasValue(true)
    {}

    Optional(const Optional& other)
    {
        *this = other;
    }

    Optional(Optional&& other) noexcept
    {
        *this = std::move(other);
    }

    Optional& operator=(const Optional& other) = default;
    Optional& operator=(Optional&& other) noexcept
    {
        m_value = std::move(other.m_value);
        m_hasValue = other.m_hasValue;
        other.m_hasValue = false;
        return *this;
    }

    void Reset()
    {
        m_value = {};
        m_hasValue = false;
    }

    Type Unwrap()
    {
        ASSERT(m_hasValue);
        m_hasValue = false;
        return std::move(m_value);
    }

    Type& GetValue()
    {
        ASSERT(m_hasValue);
        return m_value;
    }

    const Type& GetValue() const
    {
        ASSERT(m_hasValue);
        return m_value;
    }

    bool HasValue() const
    {
        return m_hasValue;
    }

    Type& operator*()
    {
        ASSERT(m_hasValue);
        return m_value;
    }

    const Type& operator*() const
    {
        ASSERT(m_hasValue);
        return m_value;
    }

    auto operator->()
    {
        ASSERT(m_hasValue);
        if constexpr(std::is_pointer_v<Type>)
        {
            ASSERT(m_value);
            return m_value;
        }
        else
        {
            return &m_value;
        }
    }

    auto operator->() const
    {
        ASSERT(m_hasValue);
        if constexpr(std::is_pointer_v<Type>)
        {
            ASSERT(m_value);
            return m_value;
        }
        else
        {
            return &m_value;
        }
    }

    explicit operator bool() const
    {
        return m_hasValue;
    }
};
