#pragma once

template<typename CharType>
class StringViewBase;

// Character string container that stores characters in contiguous
// memory buffer that can be resized to fit more characters.
// - This string implementation uses small string optimization.
// - Does not reserve more memory than actually needed.
template<typename CharType, typename Allocator = Memory::DefaultAllocator>
class StringBase
{
private:
    struct Storage : public Allocator // Empty base class optimization.
    {
        // Capacity in this string implementation does not account for null terminator.
        // When allocating/deallocating, it's important to add the null terminator size.
        // Capacity serves as a length in cases where small string optimization applies.
        u64 capacity = 0;

        union
        {
            struct
            {
                CharType* data;
                u64 length;
            } heap;

            CharType stack[sizeof(heap)] = { NullTerminator };
        };

        Allocator& GetAllocator()
        {
            return *this;
        }
    } m_storage;

public:
    static const CharType NullTerminator = '\0';
    static const u64 NullTerminatorSize = sizeof(CharType);
    static const u64 NullTerminatorCount = 1;
    static const u64 StackSize = sizeof(m_storage.stack);
    static const u32 MaxSmallLength = StackSize / sizeof(CharType) - NullTerminatorSize;

    StringBase()
    {
        Memory::FillUninitializedPattern(m_storage.stack + NullTerminatorCount, StackSize - NullTerminatorSize);
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
        *this = std::move(other);
    }

    ~StringBase()
    {
        if(!IsSmall())
        {
            ASSERT(m_storage.heap.data != nullptr);
            Memory::Deallocate<CharType>(m_storage.GetAllocator(),
                m_storage.heap.data, m_storage.capacity + NullTerminatorCount);
        }
    }

    StringBase& operator=(const CharType* text)
    {
        ConstructFromText(text, strlen(text));
        return *this;
    }

    // #todo: Make sure allocator is copied too.
    StringBase& operator=(const StringBase& other)
    {
        ASSERT(this != &other);
        ConstructFromText(other.GetData(), other.GetLength());
        return *this;
    }

    // #todo: Make sure allocator is moved too.
    StringBase& operator=(StringBase&& other) noexcept
    {
        ASSERT(this != &other);
        std::swap(m_storage.capacity, other.m_storage.capacity);
        std::swap(m_storage.heap.data, other.m_storage.heap.data);
        std::swap(m_storage.heap.length, other.m_storage.heap.length);
        return *this;
    }

    void Reserve(u64 newCapacity)
    {
        if(!IsSmallLength(newCapacity) && newCapacity > m_storage.capacity)
        {
            // Save small string data before reallocating.
            bool fromSmall = IsSmall();
            CharType stackCopy[StackSize] = { 0 };
            u64 stackLength = 0;
            if(fromSmall && m_storage.capacity > 0)
            {
                memcpy(stackCopy, m_storage.stack, StackSize);
                stackLength = m_storage.capacity;
            }

            // Allocate buffer with new capacity.
            // Reallocation should handle copying the buffer.
            const bool exactCapacity = true;
            AllocateBuffer(newCapacity, exactCapacity);
            ASSERT(m_storage.capacity >= newCapacity);

            // Copy small string data into new buffer.
            if(fromSmall)
            {
                memcpy(m_storage.heap.data, stackCopy, (stackLength + NullTerminatorCount) * sizeof(CharType));
                m_storage.heap.length = stackLength;
            }
        }
    }

    void Resize(u64 newLength, CharType fillCharacter = '\0')
    {
        Reserve(newLength);

        CharType* data = GetData();
        if(newLength > GetLength())
        {
            for(u64 i = GetLength(); i < newLength; ++i)
            {
                data[i] = fillCharacter;
            }
        }
        else if(newLength < GetLength())
        {
            Memory::FillUninitializedPattern(data + newLength + NullTerminatorCount, (GetCapacity() - newLength) * sizeof(CharType));
        }

        data[newLength] = NullTerminator;
        SetLength(newLength);
    }

    CharType* GetData()
    {
        return IsSmall() ? m_storage.stack : m_storage.heap.data;
    }

    const CharType* GetData() const
    {
        return IsSmall() ? m_storage.stack : m_storage.heap.data;
    }

    u64 GetLength() const
    {
        return IsSmall() ? m_storage.capacity : m_storage.heap.length;
    }

    u64 GetCapacity() const
    {
        return IsSmall() ? MaxSmallLength : m_storage.capacity;
    }

    bool IsEmpty() const
    {
        return GetLength() == 0;
    }
    
    bool IsSmall() const
    {
        return IsSmallLength(m_storage.capacity);
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
        // Excludes null terminator access.
        ASSERT(index < GetLength());
        return GetData()[index];
    }

    const CharType& operator[](u64 index) const
    {
        // Excludes null terminator access.
        ASSERT(index < GetLength());
        return GetData()[index];
    }

    static bool IsSmallLength(u64 length)
    {
        return length <= MaxSmallLength;
    }

private:
    void SetLength(u64 length)
    {
        if(IsSmall())
        {
            ASSERT(length <= MaxSmallLength);
            m_storage.capacity = length;
        }
        else
        {
            ASSERT(length <= m_storage.capacity);
            m_storage.heap.length = length;
        }
    }

    void ConstructFromText(const CharType* text, u64 length)
    {
        ASSERT(text != nullptr);
        if(IsSmall() && !IsSmallLength(length))
        {
            // Need to allocate memory on heap.
            const bool exactCapacity = true;
            AllocateBuffer(length, exactCapacity);
            ASSERT(m_storage.capacity == length);
        }

        if(IsSmall() && IsSmallLength(length))
        {
            // To stack from small text.
            ASSERT(length <= MaxSmallLength);
            memcpy(m_storage.stack, text, length * sizeof(CharType));
            Memory::FillUninitializedPattern(m_storage.stack + length + NullTerminatorCount,
                StackSize - length * sizeof(CharType) - NullTerminatorSize);
        }
        else
        {
            // To heap from small/large text.
            ASSERT(!IsSmall() && length <= m_storage.capacity);
            memcpy(m_storage.heap.data, text, length * sizeof(CharType));
            Memory::FillUninitializedPattern(m_storage.heap.data + length + NullTerminatorCount,
                (m_storage.capacity - length) * sizeof(CharType));
        }

        CharType* data = GetData();
        data[length] = NullTerminator;
        SetLength(length);
    }

    u64 CalculateGrowth(u64 newCapacity)
    {
        // Find the next power of two capacity (unless already power of two),
        // but not smaller than some predefined minimum starting capacity.
        // #todo: Allocator should be able to dictate capacity growth.
        return Max(64ull, NextPow2(newCapacity - 1ull));
    }

    void AllocateBuffer(u64 newCapacity, bool exactCapacity)
    {
        ASSERT_SLOW(newCapacity != m_storage.capacity);
        ASSERT(!IsSmallLength(newCapacity));

        newCapacity += NullTerminatorCount;

        if(!exactCapacity)
        {
            newCapacity = CalculateGrowth(newCapacity);
            ASSERT(newCapacity > m_storage.capacity + NullTerminatorCount);
        }

        if(IsSmall())
        {
            m_storage.heap.data = Memory::Allocate<CharType>(m_storage.GetAllocator(), newCapacity);
        }
        else
        {
            m_storage.heap.data = Memory::Reallocate<CharType>(m_storage.GetAllocator(),
                m_storage.heap.data, newCapacity, m_storage.capacity + NullTerminatorCount);
        }

        ASSERT(m_storage.heap.data != nullptr);
        m_storage.capacity = newCapacity - NullTerminatorCount;
    }
};

using String = StringBase<char>;
static_assert(sizeof(String) == 24);
