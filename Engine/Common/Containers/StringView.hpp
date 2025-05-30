#pragma once

#include "String.hpp"
#include "Common/Utility/Optional.hpp"

// String slice that does not own the data.
// Does not guarantee null-termination.
template<typename CharType>
class StringViewBase
{
    const CharType* m_data = "";
    u64 m_length = 0;

public:
    StringViewBase() = default;
    StringViewBase(const StringViewBase& other)
        : m_data(other.m_data)
        , m_length(other.m_length)
    {}

    StringViewBase(StringViewBase&& other) noexcept
    {
        *this = Move(other);
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
    {}

    StringViewBase(const CharType* data)
        : m_data(data)
        , m_length(strlen(data))
    {}

    template<typename Allocator>
    StringViewBase(const StringBase<CharType, Allocator>& string)
        : m_data(string.GetData())
        , m_length(string.GetLength())
    {}

    StringViewBase SubString(u64 start, u64 length)
    {
        start = std::min(start, m_length);
        length = std::min(length, m_length - start);
        return StringViewBase(m_data + start, length);
    }

    StringViewBase SubStringLeft(const u64 count) const
    {
        const u64 length = std::min(count, m_length);
        return StringViewBase(m_data, length);
    }

    StringViewBase SubStringRight(const u64 count) const
    {
        const u64 offset = m_length - std::min(count, m_length);
        const u64 length = m_length - offset;
        return StringViewBase(m_data + offset, length);
    }

    StringViewBase SubStringLeftAt(const u64 index) const
    {
        const u64 length = std::min(index, m_length);
        return StringViewBase(m_data, length);
    }

    StringViewBase SubStringRightAt(const u64 index) const
    {
        const u64 offset = std::min(index, m_length);
        const u64 length = m_length - offset;
        return StringViewBase(m_data + offset, length);
    }

    void RemoveLeft(const u64 count)
    {
        m_data += std::min(count, m_length);
        m_length -= count;
    }

    void RemoveRight(const u64 count)
    {
        m_length -= std::min(count, m_length);
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
        const void* result = memmem(m_data, m_length, other.m_data, other.m_length);
        if(result == nullptr)
            return {};

        return static_cast<const CharType*>(result) - m_data;
    }

    const CharType* GetData() const
    {
        ASSERT(m_data != nullptr);
        return m_data;
    }

    const CharType* GetBeginPtr() const
    {
        ASSERT(m_data != nullptr);
        return m_data;
    }

    const CharType* GetEndPtr() const
    {
        ASSERT(m_data != nullptr);
        return m_data + m_length;
    }

    u64 GetLength() const
    {
        return m_length;
    }

    u64 GetCharSize() const
    {
        return sizeof(CharType);
    }

    bool IsEmpty() const
    {
        return m_length == 0;
    }

    bool IsNullTerminated() const
    {
        return m_data[m_length] == String::NullChar;
    }

    bool operator==(const StringViewBase& other) const
    {
        if(m_length != other.m_length)
            return false;

        return std::memcmp(GetBeginPtr(), other.GetBeginPtr(), m_length * sizeof(CharType)) == 0;
    }

    const CharType* operator*() const
    {
        ASSERT(m_data != nullptr);
        ASSERT(IsNullTerminated());
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

// Used along with argument %.*s for printf().
#define STRING_VIEW_PRINTF_ARG(view) static_cast<int>(view.GetLength()), view.GetBeginPtr()
