#pragma once

#include "Memory/Memory.hpp"
#include "Memory/Allocators/DefaultAllocator.hpp"
#include "Memory/Allocators/InlineAllocator.hpp"

template<typename CharType>
class StringViewBase;

// Character string container that stores characters in contiguous
// memory buffer that can be resized to fit more characters if needed.
template<typename CharType, typename Allocator>
class StringBase
{
    using AllocationType = typename Allocator::template TypedAllocation<CharType>;
    AllocationType m_allocation;
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
        ConstructFromText(other.GetData(), other.GetLength());
    }

    StringBase(const StringBase& other)
    {
        *this = other;
    }

    StringBase(StringBase&& other) noexcept
    {
        ConstructFromText(EmptyString, 0);
        *this = std::move(other);
    }

    StringBase& operator=(const CharType* text)
    {
        ConstructFromText(text, strlen(text));
        return *this;
    }

    StringBase& operator=(const StringBase& other)
    {
        ASSERT_SLOW(this != &other);
        ConstructFromText(other.GetData(), other.m_length);
        return *this;
    }

    StringBase& operator=(StringBase&& other) noexcept
    {
        ASSERT_SLOW(this != &other);
        m_allocation = std::move(other.m_allocation);
        m_length = other.m_length;

        other.ConstructFromText(EmptyString, 0);
        return *this;
    }

    void ShrinkToFit()
    {
        m_allocation.Resize(m_length + NullCount);
    }

    void Reserve(const u64 newCapacity)
    {
        if(newCapacity + NullCount > m_allocation.GetCapacity())
        {
            m_allocation.Resize(newCapacity + NullCount);
        }
    }

    u64 Resize(const u64 newLength, const CharType fillCharacter = '\0')
    {
        if(newLength > m_length) // Grow length
        {
            if(newLength + NullCount > m_allocation.GetCapacity())
            {
                m_allocation.Resize(newLength + NullCount);
            }

            Memory::ConstructRange(
                m_allocation.GetPointer() + m_length,
                m_allocation.GetPointer() + newLength,
                fillCharacter);
        }
        else if(newLength < m_length) // Shrink length
        {
            Memory::DestructRange(
                m_allocation.GetPointer() + newLength + NullCount,
                m_allocation.GetPointer() + m_length + NullCount);
        }

        m_allocation.GetPointer()[newLength] = NullChar;
        const u64 oldLength = m_length;
        m_length = newLength;
        return oldLength;
    }

    void Clear()
    {
        if(m_length > 0)
        {
            Memory::DestructRange(
                m_allocation.GetPointer() + NullCount,
                m_allocation.GetPointer() + m_length + NullCount);

            m_allocation.GetPointer()[0] = NullChar;
            m_length = 0;
        }
    }

    CharType* GetData()
    {
        ASSERT(m_allocation.GetPointer());
        return m_allocation.GetPointer();
    }

    const CharType* GetData() const
    {
        ASSERT(m_allocation.GetPointer());
        return m_allocation.GetPointer();
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
        return GetLength() == 0;
    }

    CharType* operator*()
    {
        ASSERT(m_allocation.GetPointer());
        return m_allocation.GetPointer();
    }

    const CharType* operator*() const
    {
        ASSERT(m_allocation.GetPointer());
        return m_allocation.GetPointer();
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

    template<typename OtherAllocator>
    StringBase operator+(const StringBase<CharType, OtherAllocator>& other) const
    {
        StringBase result;
        result.Resize(m_length + other.GetLength());
        std::memcpy(result.GetData(), GetData(), m_length * sizeof(CharType));
        std::memcpy(result.GetData() + m_length, other.GetData(), other.GetLength() * sizeof(CharType));
        return result;
    }

    StringBase operator+(const CharType* other) const
    {
        ASSERT(other);
        const u64 otherLength = strlen(other);

        StringBase result;
        result.Resize(m_length + otherLength);
        std::memcpy(result.GetData(), GetData(), m_length * sizeof(CharType));
        std::memcpy(result.GetData() + m_length, other, otherLength * sizeof(CharType));
        return result;
    }

    template<typename OtherAllocator>
    void operator+=(const StringBase<CharType, OtherAllocator>& other)
    {
        const u64 oldLength = Resize(m_length + other.GetLength());
        std::memcpy(GetData() + oldLength, other.GetData(), other.GetLength() * sizeof(CharType));
    }

    void operator+=(const CharType* other)
    {
        ASSERT(other);
        const u64 otherLength = strlen(other);
        const u64 oldLength = Resize(m_length + otherLength);
        std::memcpy(GetData() + oldLength, other, otherLength * sizeof(CharType));
    }

    template<typename... Arguments>
    static StringBase Format(const CharType* format, Arguments&&... arguments)
    {
        StringBase result;
        result.Resize(std::snprintf(nullptr, 0, format, std::forward<Arguments>(arguments)...));
        std::snprintf(result.GetData(), result.GetCapacity() + NullCount,
            format, std::forward<Arguments>(arguments)...);
        return result;
    }

private:
    void ConstructFromText(const CharType* text, const u64 length)
    {
        ASSERT(text != nullptr);
        if(length + NullCount > m_allocation.GetCapacity())
        {
            m_allocation.Resize(length + NullCount);
        }

        std::memcpy(m_allocation.GetPointer(), text, length * sizeof(CharType));
        m_allocation.GetPointer()[length] = NullChar;
        m_length = length;
    }

    static u64 CalculateCapacity(const u64 newCapacity)
    {
        // Find the next power of two capacity (unless already power of two),
        // but not smaller than some predefined minimum starting capacity.
        return std::max(16ull, NextPow2(newCapacity - 1ull));
    }
};

using DefaultStringAllocator = Memory::InlineAllocator<16>;
using String = StringBase<char, DefaultStringAllocator>;
static_assert(sizeof(String) == 32);

template<u64 InlineCapacity>
using InlineString = StringBase<char, Memory::InlineAllocator<InlineCapacity>>;
static_assert(sizeof(InlineString<16>) == 32);

using HeapString = StringBase<char, Memory::DefaultAllocator>;
static_assert(sizeof(HeapString) == 24);
