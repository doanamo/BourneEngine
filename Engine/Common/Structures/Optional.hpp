#pragma once

template <typename Type>
class Optional final
{
    Type m_value;
    bool m_hasValue = false;

public:
    Optional() = default;
    Optional(const Type& value)
        : m_value(value)
        , m_hasValue(true)
    {
    }

    explicit Optional(Type&& value)
        : m_value(std::move(value))
        , m_hasValue(true)
    {
    }

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

    bool HasValue() const
    {
        return m_hasValue;
    }

    const Type& GetValue() const
    {
        ASSERT(m_hasValue);
        return m_value;
    }

    explicit operator bool() const
    {
        return m_hasValue;
    }
};
