#pragma once

#include "String.hpp"

// String slice that does not own the data.
// Does not guarantee null-termination.
template<typename CharType>
class StringViewBase
{
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
        *this = std::move(other);
    }

    StringViewBase& operator=(StringViewBase&& other) noexcept
    {
        ASSERT(this != &other);

        m_data = other.m_data;
        other.m_data = "";

        m_length = other.m_length;
        other.m_length = 0;

        return *this;
    }

    StringViewBase(const CharType* data, const u64 length)
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

    StringViewBase SubString(u64 start, u64 length)
    {
        start = Min(start, m_length);
        length = Min(length, m_length - start);
        return StringViewBase(m_data + start, length);
    }

    StringViewBase SubStringLeft(const u64 count) const
    {
        const u64 length = Min(count, m_length);
        return StringViewBase(m_data, length);
    }

    StringViewBase SubStringRight(const u64 count) const
    {
        const u64 offset = m_length - Min(count, m_length);
        const u64 length = m_length - offset;
        return StringViewBase(m_data + offset, length);
    }

    StringViewBase SubStringLeftAt(const u64 index) const
    {
        const u64 length = Min(index, m_length);
        return StringViewBase(m_data, length);
    }

    StringViewBase SubStringRightAt(const u64 index) const
    {
        const u64 offset = Min(index, m_length);
        const u64 length = m_length - offset;
        return StringViewBase(m_data + offset, length);
    }

    void RemoveLeft(const u64 count)
    {
        m_data += Min(count, m_length);
        m_length -= count;
    }

    void RemoveRight(const u64 count)
    {
        m_length -= Min(count, m_length);
    }

    bool StartsWith(const StringViewBase& other) const
    {
        if(other.m_length > m_length)
            return false;

        return std::memcmp(m_data, other.m_data, other.m_length) == 0;
    }

    bool EndsWith(const StringViewBase& other) const
    {
        if(other.m_length > m_length)
            return false;

        const u64 offset = m_length - other.m_length;
        return std::memcmp(m_data + offset, other.m_data, other.m_length) == 0;
    }

    Optional<u64> Find(const StringViewBase& other) const
    {
        const void* result = Memmem(m_data, m_length, other.m_data, other.m_length);
        if(result == nullptr)
            return {};

        return static_cast<const CharType*>(result) - m_data;
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

    const CharType& operator[](const u64 index) const
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
static_assert(sizeof(StringView) == 16);
