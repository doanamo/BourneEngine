#pragma once

#include "Memory/Memory.hpp"

// Array container that stores elements in contiguous
// memory buffer that can be resized to fit more elements.
template<typename Type, typename Allocator = Memory::DefaultAllocator>
class Array final
{
private:
    Allocator m_allocator;
    Type* m_data = nullptr;
    u64 m_capacity = 0;
    u64 m_size = 0;

public:
    Array() = default;
    ~Array()
    {
        if(m_data)
        {
            ASSERT(m_capacity > 0);
            ASSERT(m_size <= m_capacity);
            ASSERT(m_data != nullptr);
            Memory::DestructRange<Type>(m_data, m_data + m_size);
            Memory::Deallocate<Type, Allocator>(m_data, m_capacity);
        }
    }

    Array(const Array& other)
    {
        *this = other;
    }

    Array& operator=(const Array& other)
    {
        ASSERT(this != &other);

        Clear();

        if(m_capacity < other.m_size)
        {
            const bool exactCapacity = false;
            AllocateBuffer(other.m_size, exactCapacity);
            ASSERT(m_capacity >= other.m_size);
        }

        for(u64 i = 0; i < other.m_size; ++i)
        {
            Memory::Construct(m_data + i, other.m_data[i]);
        }

        m_size = other.m_size;
        return *this;
    }

    Array(Array&& other) noexcept
    {
        *this = std::move(other);
    }

    Array& operator=(Array&& other) noexcept
    {
        ASSERT(this != &other);
        std::swap(m_data, other.m_data);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_size, other.m_size);
        return *this;
    }

    void Reserve(u64 newCapacity)
    {
        if(newCapacity > m_capacity)
        {
            const bool exactCapacity = true;
            AllocateBuffer(newCapacity, exactCapacity);
            ASSERT(m_capacity >= newCapacity);
        }
    }

    template<typename... Arguments>
    void Resize(u64 newSize, Arguments... arguments)
    {
        if(newSize > m_size) // Grow size
        {
            if(newSize > m_capacity) // Grow capacity
            {
                const bool exactCapacity = true;
                AllocateBuffer(newSize, exactCapacity);
                ASSERT(m_capacity >= newSize);
            }

            Memory::ConstructRange(m_data + m_size, m_data + newSize, std::forward<Arguments>(arguments)...);
            m_size = newSize;
        }
        else if(newSize < m_size) // Shrink size (without reallocation)
        {
            Memory::DestructRange(m_data + newSize, m_data + m_size);
            m_size = newSize;
        }
    }

    template<typename... Arguments>
    void Add(Arguments&&... arguments)
    {
        u64 newSize = m_size + 1;
        if(newSize > m_capacity) // Grow capacity
        {
            const bool exactCapacity = false;
            AllocateBuffer(newSize, exactCapacity);
            ASSERT(m_capacity >= newSize);
        }

        Memory::Construct(m_data + m_size, std::forward<Arguments>(arguments)...);
        m_size = newSize;
    }
 
    void ShrinkToFit()
    {
        if(m_capacity > m_size)
        {
            const bool exactCapacity = true;
            AllocateBuffer(m_size, exactCapacity);
            ASSERT(m_capacity == m_size);
        }
    }

    void Clear()
    {
        if(m_size > 0)
        {
            Memory::DestructRange(m_data, m_data + m_size);
            m_size = 0;
        }
    }

    Type& operator[](u64 index)
    {
        ASSERT(index < m_size, "Out of bounds access with %llu index and %llu size", index, m_size);
        return m_data[index];
    }

    const Type& operator[](u64 index) const
    {
        ASSERT(index < m_size, "Out of bounds access with %llu index and %llu size", index, m_size);
        return m_data[index];
    }

    Type* GetData()
    {
        return m_data;
    }

    const Type* GetData() const
    {
        return m_data;
    }
    
    u64 GetCapacity() const
    {
        return m_capacity;
    }

    u64 GetCapacityBytes() const
    {
        return m_capacity * sizeof(Type);
    }

    u64 GetSize() const
    {
        return m_size;
    }

    u64 GetSizeBytes() const
    {
        return m_size * sizeof(Type);
    }

    u64 GetUnusedCapacity() const
    {
        ASSERT(m_capacity >= m_size);
        return m_capacity - m_size;
    }

    u64 GetUnusedCapacityBytes() const
    {
        return GetUnusedCapacity() * sizeof(Type);
    }

private:
    u64 CalculateGrowth(u64 newCapacity)
    {
        // Find the next power of two capacity (unless already power of two),
        // but not smaller than some predefined minimum starting capacity.
        return Max(4ull, NextPow2(newCapacity - 1ull));
    }

    void AllocateBuffer(u64 newCapacity, bool exactCapacity)
    {
        ASSERT_SLOW(newCapacity != m_capacity);

        if(!exactCapacity)
        {
            newCapacity = CalculateGrowth(newCapacity);
            ASSERT(newCapacity > m_capacity);
        }

        if(m_capacity == 0)
        {
            ASSERT(m_data == nullptr);
            m_data = Memory::Allocate<Type, Allocator>(newCapacity);
        }
        else
        {
            ASSERT(m_data != nullptr);
            m_data = Memory::Reallocate<Type, Allocator>(m_data, newCapacity, m_capacity);
        }

        ASSERT_SLOW(m_data != nullptr);
        m_capacity = newCapacity;
    }
};

static_assert(sizeof(Array<u8>) == 32);
static_assert(sizeof(Array<u32>) == 32);
static_assert(sizeof(Array<u64>) == 32);
