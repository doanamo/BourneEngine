#pragma once

template <typename Type>
class Optional final
{
private:
    Type m_value;
    bool m_hasValue = false;

public:
    Optional() = default;
    Optional(const Type& value)
        : m_value(value)
        , m_hasValue(true)
    {
    }

    Optional(Type&& value)
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

    Optional& operator=(const Optional& other)
    {
        m_hasValue = other.m_hasValue;
        m_value = other.m_value;
        return *this;
    }

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
        return m_value;
    }

    operator bool() const
    {
        return m_hasValue;
    }
};
