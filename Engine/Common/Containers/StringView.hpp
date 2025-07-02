#pragma once

#include "String.hpp"
#include "StringShared.hpp"

// String slice that does not own the data.
// Does not guarantee null-termination.
template<typename CharType>
class StringViewBase : public StringShared<StringViewBase<CharType>, CharType>
{
    static_assert(std::is_trivial_v<CharType>);
    const CharType* m_data = "";
    u64 m_length = 0;

public:
    StringViewBase() = default;
    StringViewBase(const StringViewBase& other)
        : m_data(other.m_data)
        , m_length(other.m_length)
    {
    }

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

    StringViewBase(const CharType* data)
        : m_data(data)
        , m_length(std::strlen(data))
    {
    }

    StringViewBase(const CharType* data, const u64 length)
        : m_data(data)
        , m_length(length)
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

    bool IsNullTerminated() const
    {
        return m_data[m_length] == String::NullChar;
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
