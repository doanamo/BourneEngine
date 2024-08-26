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
            static_assert(sizeof(stack) == 16);
        };

        Allocator& GetAllocator()
        {
            return *this;
        }
    } m_storage;

public:
    static const CharType NullTerminator = '\0';
    static const u64 NullTerminatorSize = 1;
    static const u32 MaxSmallLength = sizeof(m_storage.stack) - NullTerminatorSize;

    StringBase()
    {
        Memory::FillUninitializedPattern(m_storage.stack + NullTerminatorSize,
            sizeof(m_storage.stack) - NullTerminatorSize);
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
            ASSERT(m_storage.heap.data != nullptr);
            Memory::Deallocate<CharType>(m_storage.GetAllocator(),
                m_storage.heap.data, m_storage.capacity + NullTerminatorSize);
        }
    }

    StringBase& operator=(const CharType* text)
    {
        ConstructFromData(text, strlen(text));
        return *this;
    }

    // #todo: Make sure allocator is copied too.
    StringBase& operator=(const StringBase& other)
    {
        ASSERT(this != &other);
        ConstructFromData(other.GetData(), other.GetLength());
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
            bool fromSmall = IsSmall();
            CharType stack[sizeof(m_storage.stack)];
            u64 stackLength = 0;
            if(fromSmall && m_storage.capacity > 0)
            {
                memcpy(stack, m_storage.stack, sizeof(m_storage.stack));
                stackLength = m_storage.capacity;
            }

            const bool exactCapacity = true;
            AllocateBuffer(newCapacity, exactCapacity);
            ASSERT(m_storage.capacity >= newCapacity);

            if(fromSmall)
            {
                if(stackLength > 0)
                {
                    memcpy(m_storage.heap.data, stack, stackLength + NullTerminatorSize);
                    m_storage.heap.length = stackLength;
                }
                else
                {
                    m_storage.heap.data[0] = NullTerminator;
                    m_storage.heap.length = 0;
                }
            }
        }
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
        return IsSmall() ? sizeof(m_storage.stack) - NullTerminatorSize : m_storage.capacity;
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
        return length < sizeof(m_storage.stack);
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
            ASSERT(m_storage.capacity == length);
        }

        if(IsSmall() && IsSmallLength(length))
        {
            // To stack from small text
            ASSERT(length <= sizeof(m_storage.stack) - NullTerminatorSize);
            memcpy(m_storage.stack, data, length);
            m_storage.stack[length] = NullTerminator;
            Memory::FillUninitializedPattern(m_storage.stack + length + NullTerminatorSize,
                sizeof(m_storage.stack) - length - NullTerminatorSize);
            m_storage.capacity = length;
        }
        else
        {
            // To heap from small/large text
            ASSERT(!IsSmall() && length <= m_storage.capacity);
            memcpy(m_storage.heap.data, data, length);
            m_storage.heap.data[length] = NullTerminator;
            Memory::FillUninitializedPattern(m_storage.heap.data + length + NullTerminatorSize,
                m_storage.capacity - length);
            m_storage.heap.length = length;
        }
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

        newCapacity += NullTerminatorSize;

        if(!exactCapacity)
        {
            newCapacity = CalculateGrowth(newCapacity);
            ASSERT(newCapacity > m_storage.capacity + NullTerminatorSize);
        }

        if(IsSmall())
        {
            m_storage.heap.data = Memory::Allocate<CharType>(m_storage.GetAllocator(), newCapacity);
        }
        else
        {
            m_storage.heap.data = Memory::Reallocate<CharType>(m_storage.GetAllocator(),
                m_storage.heap.data, newCapacity, m_storage.capacity + NullTerminatorSize);
        }

        ASSERT(m_storage.heap.data != nullptr);
        m_storage.capacity = newCapacity - NullTerminatorSize;
    }
};

using String = StringBase<char>;
static_assert(sizeof(String) == 24);
