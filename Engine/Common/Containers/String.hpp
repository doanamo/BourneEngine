#pragma once

#include "Memory/Memory.hpp"
#include "Memory/Allocators/DefaultAllocator.hpp"
#include "Memory/Allocators/InlineAllocator.hpp"

template<typename CharType>
class StringViewBase;

// Character string container that stores characters in contiguous
// memory buffer that can be resized to fit more characters.
template<typename CharType, typename Allocator = Memory::DefaultAllocator>
class StringBase
{
private:
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
        std::swap(m_allocation, other.m_allocation);
        std::swap(m_length, other.m_length);
        return *this;
    }

    void ShrinkToFit()
    {
        EnsureCapacity(m_length + NullTerminatorCount, CapacityMode::ForceExact);
    }

    void Reserve(u64 newLength)
    {
        EnsureCapacity(newLength + NullTerminatorCount, CapacityMode::GrowExact);
    }

    void Resize(u64 newLength, CharType fillCharacter = '\0')
    {
        if(newLength > m_length) // Grow length
        {
            EnsureCapacity(newLength + NullTerminatorCount, CapacityMode::GrowExact);

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
        ASSERT_SLOW(m_allocation.GetCapacity() != 0);
        ASSERT_SLOW(m_allocation.GetCapacity() >= m_length + NullTerminatorCount);
        return m_allocation.GetCapacity() - NullTerminatorCount;
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
    void ConstructFromText(const CharType* text, u64 length)
    {
        ASSERT(text != nullptr);
        EnsureCapacity(length + NullTerminatorCount, CapacityMode::GrowExact);
        memcpy(m_allocation.GetPointer(), text, length * sizeof(CharType));

        m_allocation.GetPointer()[length] = NullTerminator;
        m_length = length;
    }

    enum class CapacityMode
    {
        Grow,
        GrowExact,
        ForceExact,
    };

    u64 CalculateCapacity(u64 newCapacity)
    {
        // Find the next power of two capacity (unless already power of two),
        // but not smaller than some predefined minimum starting capacity.
        return Max(16ull, NextPow2(newCapacity - 1ull));
    }

    // #todo: Make this a generic allocator function
    void EnsureCapacity(u64 newCapacity, CapacityMode mode)
    {
        ASSERT(newCapacity != 0);
        if(mode != CapacityMode::ForceExact)
        {
            if(newCapacity < m_allocation.GetCapacity())
                return;
        }

        if(mode == CapacityMode::Grow)
        {
            newCapacity = CalculateCapacity(newCapacity);
        }

        if(m_allocation.GetCapacity() == 0)
        {
            m_allocation.Allocate(newCapacity);
        }
        else
        {
            m_allocation.Reallocate(newCapacity);
        }

        ASSERT_SLOW(m_allocation.GetPointer() != nullptr);
        ASSERT_SLOW(m_allocation.GetCapacity() >= newCapacity);
    }
};

using DefaultStringAllocator = Memory::InlineAllocator<16>;
using String = StringBase<char, DefaultStringAllocator>;
static_assert(sizeof(String) == 32);

template<u64 InlineCapacity>
using InlineString = StringBase<char, Memory::InlineAllocator<InlineCapacity>>;
using HeapString = StringBase<char, Memory::DefaultAllocator>;
