#pragma once

#include "Memory/Memory.hpp"
#include "Memory/Allocators/Default.hpp"
#include "Memory/Allocators/Inline.hpp"

template<typename CharType>
class StringViewBase;

// Character string container that stores characters in contiguous
// memory buffer that can be resized to fit more characters if needed.
template<typename CharType, typename Allocator>
class StringBase
{
    static_assert(std::is_trivial_v<CharType>);
    using Allocation = typename Allocator::template TypedAllocation<CharType>;
    Allocation m_allocation;
    u64 m_length = 0;

public:
    static constexpr u64 CharSize = sizeof(CharType);
    static constexpr u64 NullCount = 1;
    static constexpr CharType NullChar = '\0';
    static constexpr CharType EmptyString[NullCount] = { NullChar };

    StringBase()
    {
        ConstructFromText(EmptyString, 0);
    }

    StringBase(const CharType* text)
    {
        ConstructFromText(text, strlen(text));
    }

    StringBase(const StringViewBase<CharType>& other)
    {
        ConstructFromText(other.GetBeginPtr(), other.GetLength());
    }

    explicit StringBase(const StringBase& other)
    {
        *this = other;
    }

    template<typename OtherAllocator>
    explicit StringBase(const StringBase<CharType, OtherAllocator>& other)
    {
        ConstructFromText(other.GetData(), other.GetLength());
    }

    explicit StringBase(StringBase&& other) noexcept
    {
        ConstructFromText(EmptyString, 0);
        *this = Move(other);
    }

    StringBase& operator=(const CharType* text)
    {
        ConstructFromText(text, strlen(text));
        return *this;
    }

    StringBase& operator=(const StringViewBase<CharType>& other)
    {
        ConstructFromText(other.GetBeginPtr(), other.GetLength());
        return *this;
    }

    StringBase& operator=(const StringBase& other)
    {
        ASSERT_SLOW(this != &other);
        ConstructFromText(other.GetData(), other.GetLength());
        return *this;
    }

    template<typename OtherAllocator>
    StringBase& operator=(const StringBase<CharType, OtherAllocator>& other)
    {
        ASSERT_SLOW(this != &other);
        ConstructFromText(other.GetData(), other.GetLength());
        return *this;
    }

    StringBase& operator=(StringBase&& other) noexcept
    {
        m_allocation = Move(other.m_allocation);
        m_length = other.m_length;
        other.ConstructFromText(EmptyString, 0);
        return *this;
    }

    template<typename OtherAllocator>
    StringBase& operator=(StringBase<CharType, OtherAllocator>&& other) noexcept
    {
        ConstructFromText(other.GetData(), other.GetLength());
        return *this;
    }

    void ShrinkToFit()
    {
        const u64 usedCapacity = m_length + NullCount;
        m_allocation.Resize(m_length ? usedCapacity : 0, usedCapacity);
    }

    void Reserve(const u64 length, const bool exact = true)
    {
        u64 newCapacity = length ? length + NullCount : 0;
        if(newCapacity > m_allocation.GetCapacity())
        {
            if(!exact)
            {
                newCapacity = CalculateCapacity(newCapacity);
            }

            const u64 usedCapacity = m_length + NullCount;
            m_allocation.Resize(newCapacity, usedCapacity);
        }
    }

    void Resize(const u64 length, const CharType fillCharacter = ' ')
    {
        if(length > m_length)
        {
            Reserve(length);
            Memory::ConstructRange(
                m_allocation.GetPointer() + m_length,
                m_allocation.GetPointer() + length,
                fillCharacter);
        }

        if(CharType* data = m_allocation.GetPointer())
        {
            data[length] = NullChar;
        }

        m_length = length;
    }

    void Clear()
    {
        if(CharType* data = m_allocation.GetPointer())
        {
            data[0] = NullChar;
        }

        m_length = 0;
    }

    CharType* GetData()
    {
        if(CharType* data = m_allocation.GetPointer())
        {
            return data;
        }

        ASSERT_SLOW(EmptyString[0] == NullChar);
        return const_cast<CharType*>(EmptyString);
    }

    const CharType* GetData() const
    {
        if(const CharType* data = m_allocation.GetPointer())
        {
            return data;
        }

        ASSERT_SLOW(EmptyString[0] == NullChar);
        return EmptyString;
    }

    u64 GetLength() const
    {
        return m_length;
    }

    u64 GetCapacity() const
    {
        const u64 capacity = m_allocation.GetCapacity();
        return capacity != 0 ? capacity - NullCount : 0;
    }

    bool IsEmpty() const
    {
        return m_length == 0;
    }

    bool operator==(const CharType* other) const
    {
        if(m_length != std::strlen(other))
            return false;

        return std::memcmp(GetData(), other, m_length * sizeof(CharType)) == 0;
    }

    template<typename OtherAllocator>
    bool operator==(const StringBase<CharType, OtherAllocator>& other) const
    {
        if(m_length != other.m_length)
            return false;

        return std::memcmp(GetData(), other.GetData(), m_length * sizeof(CharType)) == 0;
    }

    template<typename OtherAllocator>
    StringBase operator+(const StringBase<CharType, OtherAllocator>& other) const
    {
        const u64 length = m_length + other.GetLength();

        StringBase result;
        result.Reserve(length);
        if(CharType* resultData = result.m_allocation.GetPointer())
        {
            std::memcpy(resultData, GetData(), m_length * sizeof(CharType));
            std::memcpy(resultData + m_length, other.GetData(), other.GetLength() * sizeof(CharType));
            resultData[length] = NullChar;
        }

        result.m_length = length;
        return result;
    }

    StringBase operator+(const CharType* other) const
    {
        ASSERT(other);
        const u64 otherLength = strlen(other);
        const u64 length = m_length + otherLength;

        StringBase result;
        result.Reserve(length);
        if(CharType* resultData = result.m_allocation.GetPointer())
        {
            std::memcpy(resultData, GetData(), m_length * sizeof(CharType));
            std::memcpy(resultData + m_length, other, otherLength * sizeof(CharType));
            resultData[length] = NullChar;
        }

        result.m_length = length;
        return result;
    }

    template<typename OtherAllocator>
    void operator+=(const StringBase<CharType, OtherAllocator>& other)
    {
        const u64 oldLength = m_length;
        const u64 newLength = m_length + other.GetLength();

        Reserve(newLength, false);
        if(CharType* data = m_allocation.GetPointer())
        {
            std::memcpy(data + oldLength, other.GetData(), other.GetLength() * sizeof(CharType));
            data[newLength] = NullChar;
        }

        m_length = newLength;
    }

    void operator+=(const CharType* other)
    {
        ASSERT(other);
        const u64 oldLength = m_length;
        const u64 otherLength = strlen(other);
        const u64 newLength = m_length + otherLength;

        Reserve(newLength, false);
        if(CharType* data = m_allocation.GetPointer())
        {
            std::memcpy(data + oldLength, other, otherLength * sizeof(CharType));
            data[newLength] = NullChar;
        }

        m_length = newLength;
    }

    template<typename... Arguments>
    static StringBase Format(const CharType* format, Arguments&&... arguments)
    {
        StringBase result;
        result.AppendInternal(true, format, Forward<Arguments>(arguments)...);
        return result;
    }

    template<typename... Arguments>
    void Append(const CharType* format, Arguments&&... arguments)
    {
        AppendInternal(false, format, Forward<Arguments>(arguments)...);
    }

    CharType* operator*()
    {
        return GetData();
    }

    const CharType* operator*() const
    {
        return GetData();
    }

    CharType& operator[](u64 index)
    {
        ASSERT(m_allocation.GetPointer());
        ASSERT(index <= m_length, "Out of bounds access with %llu index and %llu length", index, m_length);
        return GetData()[index];
    }

    const CharType& operator[](u64 index) const
    {
        ASSERT(m_allocation.GetPointer());
        ASSERT(index <= m_length, "Out of bounds access with %llu index and %llu length", index, m_length);
        return GetData()[index];
    }

private:
    void ConstructFromText(const CharType* text, const u64 length)
    {
        Reserve(length);
        if(CharType* data = m_allocation.GetPointer())
        {
            std::memcpy(data, text, length * sizeof(CharType));
            data[length] = NullChar;
        }

        m_length = length;
    }

    template<typename... Arguments>
    void AppendInternal(const bool reserveExact, const CharType* format, Arguments&&... arguments)
    {
        ASSERT(format);
        const u64 length = std::snprintf(nullptr, 0, format, Forward<Arguments>(arguments)...);

        Reserve(m_length + length, reserveExact);
        CharType* data = m_allocation.GetPointer();
        ASSERT(data);

        data += m_length;
        std::snprintf(data, length + NullCount, format, Forward<Arguments>(arguments)...);
        data[length] = NullChar;
        m_length += length;
    }

    static u64 CalculateCapacity(const u64 newCapacity)
    {
        ASSERT(newCapacity != 0);

        // Find the next power of two capacity (unless already power of two),
        // but not smaller than some predefined minimum starting capacity.
        return std::max(16ull, NextPow2(newCapacity - 1ull));
    }
};

using DefaultStringAllocator = Memory::Allocators::Inline<16>;
using String = StringBase<char, DefaultStringAllocator>;
static_assert(sizeof(String) == 32);

template<u64 InlineCapacity = 16>
using InlineString = StringBase<char, Memory::Allocators::Inline<InlineCapacity>>;
static_assert(sizeof(InlineString<16>) == 32);

using HeapString = StringBase<char, Memory::Allocators::Default>;
static_assert(sizeof(HeapString) == 24);
