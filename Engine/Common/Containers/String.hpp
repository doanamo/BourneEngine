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
    static constexpr CharType NullTerminator = '\0';
    static constexpr CharType EmptyString[1] = { NullTerminator };
    static constexpr u64 NullTerminatorSize = sizeof(CharType);
    static constexpr u64 NullTerminatorCount = 1;

    StringBase()
    {
        ConstructFromText(EmptyString, 0);
    }

    explicit StringBase(const CharType* text)
    {
        ConstructFromText(text, strlen(text));
    }

    explicit StringBase(const StringViewBase<CharType>& other)
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
        m_allocation.Resize(m_length + NullTerminatorCount);
    }

    void Reserve(const u64 newCapacity)
    {
        if(newCapacity + NullTerminatorCount > m_allocation.GetCapacity())
        {
            m_allocation.Resize(newCapacity + NullTerminatorCount);
        }
    }

    void Resize(const u64 newLength, const CharType fillCharacter = '\0')
    {
        if(newLength > m_length) // Grow length
        {
            if(newLength + NullTerminatorCount > m_allocation.GetCapacity())
            {
                m_allocation.Resize(newLength + NullTerminatorCount);
            }

            Memory::ConstructRange(
                m_allocation.GetPointer() + m_length,
                m_allocation.GetPointer() + newLength,
                fillCharacter);
        }
        else if(newLength < m_length) // Shrink length
        {
            Memory::DestructRange(
                m_allocation.GetPointer() + newLength + NullTerminatorCount,
                m_allocation.GetPointer() + m_length + NullTerminatorCount);
        }

        m_allocation.GetPointer()[newLength] = NullTerminator;
        m_length = newLength;
    }

    void Clear()
    {
        if(m_length > 0)
        {
            Memory::DestructRange(
                m_allocation.GetPointer() + NullTerminatorCount,
                m_allocation.GetPointer() + m_length + NullTerminatorCount);

            m_allocation.GetPointer()[0] = NullTerminator;
            m_length = 0;
        }
    }

    CharType* GetData()
    {
        ASSERT_SLOW(m_allocation.GetPointer());
        return m_allocation.GetPointer();
    }

    const CharType* GetData() const
    {
        ASSERT_SLOW(m_allocation.GetPointer());
        return m_allocation.GetPointer();
    }

    u64 GetLength() const
    {
        return m_length;
    }

    u64 GetCapacity() const
    {
        const u64 capacity = m_allocation.GetCapacity();
        return capacity != 0 ? capacity - NullTerminatorCount : 0;
    }

    bool IsEmpty() const
    {
        return GetLength() == 0;
    }

    CharType* operator*()
    {
        ASSERT_SLOW(m_allocation.GetPointer());
        return m_allocation.GetPointer();
    }

    const CharType* operator*() const
    {
        ASSERT_SLOW(m_allocation.GetPointer());
        return m_allocation.GetPointer();
    }

    CharType& operator[](u64 index)
    {
        ASSERT_SLOW(m_allocation.GetPointer());
        ASSERT(index <= m_length, "Out of bounds access with %llu index and %llu length", index, m_length);
        return GetData()[index];
    }

    const CharType& operator[](u64 index) const
    {
        ASSERT_SLOW(m_allocation.GetPointer());
        ASSERT(index <= m_length, "Out of bounds access with %llu index and %llu length", index, m_length);
        return GetData()[index];
    }

    template<typename... Arguments>
    static StringBase Format(const CharType* format, Arguments&&... arguments)
    {
        StringBase result;
        result.Resize(std::snprintf(nullptr, 0, format, std::forward<Arguments>(arguments)...));
        std::snprintf(result.GetData(), result.GetCapacity() + NullTerminatorCount,
            format, std::forward<Arguments>(arguments)...);
        return result;
    }

private:
    void ConstructFromText(const CharType* text, const u64 length)
    {
        ASSERT(text != nullptr);
        if(length + NullTerminatorCount > m_allocation.GetCapacity())
        {
            m_allocation.Resize(length + NullTerminatorCount);
        }

        std::memcpy(m_allocation.GetPointer(), text, length * sizeof(CharType));
        m_allocation.GetPointer()[length] = NullTerminator;
        m_length = length;
    }

    static u64 CalculateCapacity(const u64 newCapacity)
    {
        // Find the next power of two capacity (unless already power of two),
        // but not smaller than some predefined minimum starting capacity.
        return Max(16ull, NextPow2(newCapacity - 1ull));
    }
};

using DefaultStringAllocator = Memory::InlineAllocator<16>;
using String = StringBase<char, DefaultStringAllocator>;
static_assert(sizeof(String) == 32);

template<u64 InlineCapacity>
using InlineString = StringBase<char, Memory::InlineAllocator<InlineCapacity>>;
using HeapString = StringBase<char, Memory::DefaultAllocator>;
