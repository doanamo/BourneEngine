#pragma once

#include "String.hpp"

// String reference that does not own the data.
// Does not guarantee null-termination.
template<typename CharType>
class StringViewBase
{
private:
    const CharType* m_data = "";
    u64 m_length = 0;

public:
    StringViewBase() = default;

    StringViewBase(const StringViewBase& other) :
        m_data(other.m_data),
        m_length(other.m_length)
    {
    }

    StringViewBase(StringViewBase&& other) noexcept
    {
        std::swap(m_data, other.m_data);
        std::swap(m_length, other.m_length);
    }

    StringViewBase(const CharType* data, u64 length)
        : m_data(data)
        , m_length(length)
    {
        ASSERT(m_length >= 0);
    }

    StringViewBase(const CharType* data)
        : m_data(data)
        , m_length(strlen(data))
    {
        ASSERT(m_length >= 0);
    }

    StringViewBase(const StringBase<CharType>& string)
        : m_data(string.GetData())
        , m_length(string.GetSize())
    {
        ASSERT(m_length >= 0);
    }

    const CharType* GetData() const
    {
        ASSERT(m_data != nullptr);
        return m_data;
    }

    u64 GetLength() const
    {
        ASSERT(m_length >= 0);
        return m_length;
    }

    bool IsEmpty() const
    {
        ASSERT(m_length >= 0);
        return m_length == 0;
    }

    const CharType* operator*() const
    {
        ASSERT(m_data != nullptr);
        return m_data;
    }

    const CharType& operator[](u64 index) const
    {
        ASSERT(m_data != nullptr);
        ASSERT(index < m_length);
        return m_data[index];
    }

    StringBase<CharType> ToString() const
    {
        ASSERT(m_data != nullptr);
        ASSERT(m_length >= 0);
        return StringBase<CharType>(*this);
    }
};

using StringView = StringViewBase<char>;
