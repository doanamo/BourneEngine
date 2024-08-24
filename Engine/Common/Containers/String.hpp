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
    Allocator m_allocator;

    // Capacity in this string implementation does not account for null terminator.
    // When allocating/deallocating, it's important to add the null terminator size.
    // Capacity serves as a length in cases where small string optimization applies.
    u64 m_capacity = 0;

    union
    {
        struct
        {
            CharType* data;
            u64 length;
        } m_heap;

        CharType m_stack[sizeof(m_heap)] = { NullTerminator };
        static_assert(sizeof(m_stack) == 16);
    };

public:
    static const CharType NullTerminator = '\0';
    static const u64 NullTerminatorSize = 1;
    static const u32 MaxSmallLength = sizeof(m_stack) - NullTerminatorSize;

    StringBase()
    {
        Memory::FillUninitializedPattern(m_stack + NullTerminatorSize, sizeof(m_stack) - NullTerminatorSize);
    }

    StringBase(const CharType* text)
    {
        ConstructFromData(text, strlen(text));
    }

    StringBase(const StringViewBase<CharType>& other)
    {
        ConstructFromData(other.GetData(), other.GetLength());
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
            ASSERT(m_heap.data != nullptr);
            Memory::Deallocate<CharType>(m_allocator, m_heap.data, m_capacity + NullTerminatorSize);
        }
    }

    StringBase& operator=(const CharType* text)
    {
        ConstructFromData(text, strlen(text));
        return *this;
    }

    StringBase& operator=(const StringBase& other)
    {
        ASSERT(this != &other);
        ConstructFromData(other.GetData(), other.GetLength());
        return *this;
    }

    StringBase& operator=(StringBase&& other) noexcept
    {
        ASSERT(this != &other);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_heap.data, other.m_heap.data);
        std::swap(m_heap.length, other.m_heap.length);
        return *this;
    }

    void Reserve(u64 newCapacity)
    {
        if(!IsSmallLength(newCapacity) && newCapacity > m_capacity)
        {
            bool fromSmall = IsSmall();
            CharType stack[sizeof(m_stack)];
            u64 stackLength = 0;
            if(fromSmall && m_capacity > 0)
            {
                memcpy(stack, m_stack, sizeof(m_stack));
                stackLength = m_capacity;
            }

            const bool exactCapacity = true;
            AllocateBuffer(newCapacity, exactCapacity);
            ASSERT(m_capacity >= newCapacity);

            if(fromSmall)
            {
                if(stackLength > 0)
                {
                    memcpy(m_heap.data, stack, stackLength + NullTerminatorSize);
                    m_heap.length = stackLength;
                }
                else
                {
                    m_heap.data[0] = NullTerminator;
                    m_heap.length = 0;
                }
            }
        }
    }

    CharType* GetData()
    {
        return IsSmall() ? m_stack : m_heap.data;
    }

    const CharType* GetData() const
    {
        return IsSmall() ? m_stack : m_heap.data;
    }

    u64 GetLength() const
    {
        return IsSmall() ? m_capacity : m_heap.length;
    }

    u64 GetCapacity() const
    {
        return IsSmall() ? sizeof(m_stack) - NullTerminatorSize : m_capacity;
    }

    bool IsEmpty() const
    {
        return GetLength() == 0;
    }
    
    bool IsSmall() const
    {
        return IsSmallLength(m_capacity);
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
        ASSERT(index <= GetLength());
        return GetData()[index];
    }

    const CharType& operator[](u64 index) const
    {
        ASSERT(index <= GetLength());
        return GetData()[index];
    }

    static bool IsSmallLength(u64 length)
    {
        return length < sizeof(m_stack);
    }

private:
    void ConstructFromData(const CharType* data, u64 length)
    {
        ASSERT(data != nullptr);
        if(IsSmall() && !IsSmallLength(length))
        {
            // Need to allocate memory on heap
            const bool exactCapacity = true;
            AllocateBuffer(length, exactCapacity);
            ASSERT(m_capacity == length);
        }

        if(IsSmall() && IsSmallLength(length))
        {
            // To stack from small text
            ASSERT(length <= sizeof(m_stack) - NullTerminatorSize);
            memcpy(m_stack, data, length);
            m_stack[length] = NullTerminator;
            Memory::FillUninitializedPattern(m_stack + length + NullTerminatorSize, sizeof(m_stack) - length - NullTerminatorSize);
            m_capacity = length;
        }
        else
        {
            // To heap from small/large text
            ASSERT(!IsSmall() && length <= m_capacity);
            memcpy(m_heap.data, data, length);
            m_heap.data[length] = NullTerminator;
            Memory::FillUninitializedPattern(m_heap.data + length + NullTerminatorSize, m_capacity - length);
            m_heap.length = length;
        }
    }

    u64 CalculateGrowth(u64 newCapacity)
    {
        // Find the next power of two capacity (unless already power of two),
        // but not smaller than some predefined minimum starting capacity.
        return Max(64ull, NextPow2(newCapacity - 1ull));
    }

    void AllocateBuffer(u64 newCapacity, bool exactCapacity)
    {
        ASSERT_SLOW(newCapacity != m_capacity);
        ASSERT(!IsSmallLength(newCapacity));

        newCapacity += NullTerminatorSize;

        if(!exactCapacity)
        {
            newCapacity = CalculateGrowth(newCapacity);
            ASSERT(newCapacity > m_capacity + NullTerminatorSize);
        }

        if(IsSmall())
        {
            m_heap.data = Memory::Allocate<CharType>(m_allocator, newCapacity);
        }
        else
        {
            m_heap.data = Memory::Reallocate<CharType>(m_allocator, m_heap.data, newCapacity, m_capacity + NullTerminatorSize);
        }

        ASSERT(m_heap.data != nullptr);
        m_capacity = newCapacity - NullTerminatorSize;
    }
};

using String = StringBase<char>;
static_assert(sizeof(String) == 32);
