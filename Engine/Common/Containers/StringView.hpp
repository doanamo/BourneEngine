#pragma once

#include "String.hpp"

// String slice that does not own the data.
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
        ASSERT(this != &other);
        std::swap(m_data, other.m_data);
        std::swap(m_length, other.m_length);
    }

    StringViewBase(const CharType* data, u64 length)
        : m_data(data)
        , m_length(length)
    {
    }

    StringViewBase(const CharType* data)
        : m_data(data)
        , m_length(strlen(data))
    {
    }

    template<typename Allocator>
    StringViewBase(const StringBase<CharType, Allocator>& string)
        : m_data(string.GetData())
        , m_length(string.GetLength())
    {
    }

    const CharType* GetData() const
    {
        ASSERT(m_data != nullptr);
        return m_data;
    }

    u64 GetLength() const
    {
        return m_length;
    }

    bool IsEmpty() const
    {
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

    template<typename Allocator = DefaultStringAllocator>
    StringBase<CharType, Allocator> ToString() const
    {
        ASSERT(m_data != nullptr);
        return StringBase<CharType, Allocator>(*this);
    }
};

using StringView = StringViewBase<char>;
