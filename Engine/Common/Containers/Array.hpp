#pragma once

#include "Memory/Memory.hpp"

// Array container that stores elements in contiguous
// memory buffer that can be resized to fit more elements.
template<typename Type, typename Allocator = Memory::DefaultAllocator>
class Array final
{
private:
    struct Storage : public Allocator // Empty base class optimization.
    {
        Type* data = nullptr;
        u64 capacity = 0;
        u64 size = 0;

        Allocator& GetAllocator()
        {
            return *this;
        }
    } m_storage;

public:
    Array() = default;

    Array(const Array& other)
    {
        *this = other;
    }

    Array(Array&& other) noexcept
    {
        *this = std::move(other);
    }

    ~Array()
    {
        if(m_storage.data)
        {
            ASSERT(m_storage.capacity > 0);
            ASSERT(m_storage.size <= m_storage.capacity);
            ASSERT(m_storage.data != nullptr);
            Memory::DestructRange<Type>(m_storage.data, m_storage.data + m_storage.size);
            Memory::Deallocate<Type>(m_storage.GetAllocator(), m_storage.data, m_storage.capacity);
        }
    }

    // #todo: Make sure allocator is copied too.
    Array& operator=(const Array& other)
    {
        ASSERT(this != &other);

        Clear();

        if(m_storage.capacity < other.m_storage.size)
        {
            const bool exactCapacity = false;
            AllocateBuffer(other.m_storage.size, exactCapacity);
            ASSERT(m_storage.capacity >= other.m_storage.size);
        }

        for(u64 i = 0; i < other.m_storage.size; ++i)
        {
            Memory::Construct(m_storage.data + i, other.m_storage.data[i]);
        }

        m_storage.size = other.m_storage.size;
        return *this;
    }

    // #todo: Make sure allocator is moved too.
    Array& operator=(Array&& other) noexcept
    {
        ASSERT(this != &other);
        std::swap(m_storage.data, other.m_storage.data);
        std::swap(m_storage.capacity, other.m_storage.capacity);
        std::swap(m_storage.size, other.m_storage.size);
        return *this;
    }

    void Reserve(u64 newCapacity)
    {
        if(newCapacity > m_storage.capacity)
        {
            const bool exactCapacity = true;
            AllocateBuffer(newCapacity, exactCapacity);
            ASSERT(m_storage.capacity >= newCapacity);
        }
    }

    template<typename... Arguments>
    void Resize(u64 newSize, Arguments... arguments)
    {
        if(newSize > m_storage.size) // Grow size
        {
            if(newSize > m_storage.capacity) // Grow capacity
            {
                const bool exactCapacity = true;
                AllocateBuffer(newSize, exactCapacity);
                ASSERT(m_storage.capacity >= newSize);
            }

            Memory::ConstructRange(m_storage.data + m_storage.size,
                m_storage.data + newSize, std::forward<Arguments>(arguments)...);
            m_storage.size = newSize;
        }
        else if(newSize < m_storage.size) // Shrink size (without reallocation)
        {
            Memory::DestructRange(m_storage.data + newSize, m_storage.data + m_storage.size);
            m_storage.size = newSize;
        }
    }

    template<typename... Arguments>
    void Add(Arguments&&... arguments)
    {
        u64 newSize = m_storage.size + 1;
        if(newSize > m_storage.capacity) // Grow capacity
        {
            const bool exactCapacity = false;
            AllocateBuffer(newSize, exactCapacity);
            ASSERT(m_storage.capacity >= newSize);
        }

        Memory::Construct(m_storage.data + m_storage.size, std::forward<Arguments>(arguments)...);
        m_storage.size = newSize;
    }
 
    void ShrinkToFit()
    {
        if(m_storage.capacity > m_storage.size)
        {
            const bool exactCapacity = true;
            AllocateBuffer(m_storage.size, exactCapacity);
            ASSERT(m_storage.capacity == m_storage.size);
        }
    }

    void Clear()
    {
        if(m_storage.size > 0)
        {
            Memory::DestructRange(m_storage.data, m_storage.data + m_storage.size);
            m_storage.size = 0;
        }
    }

    Type& operator[](u64 index)
    {
        ASSERT(index < m_storage.size, "Out of bounds access with %llu index and %llu size", index, m_storage.size);
        return m_storage.data[index];
    }

    const Type& operator[](u64 index) const
    {
        ASSERT(index < m_storage.size, "Out of bounds access with %llu index and %llu size", index, m_storage.size);
        return m_storage.data[index];
    }

    Type* GetData()
    {
        return m_storage.data;
    }

    const Type* GetData() const
    {
        return m_storage.data;
    }
    
    u64 GetCapacity() const
    {
        return m_storage.capacity;
    }

    u64 GetCapacityBytes() const
    {
        return m_storage.capacity * sizeof(Type);
    }

    u64 GetSize() const
    {
        return m_storage.size;
    }

    u64 GetSizeBytes() const
    {
        return m_storage.size * sizeof(Type);
    }

    u64 GetUnusedCapacity() const
    {
        ASSERT(m_storage.capacity >= m_storage.size);
        return m_storage.capacity - m_storage.size;
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
        // #todo: Allocator should be able to dictate capacity growth.
        return Max(4ull, NextPow2(newCapacity - 1ull));
    }

    void AllocateBuffer(u64 newCapacity, bool exactCapacity)
    {
        ASSERT_SLOW(newCapacity != m_storage.capacity);

        if(!exactCapacity)
        {
            newCapacity = CalculateGrowth(newCapacity);
            ASSERT(newCapacity > m_storage.capacity);
        }

        if(m_storage.capacity == 0)
        {
            ASSERT(m_storage.data == nullptr);
            m_storage.data = Memory::Allocate<Type>(m_storage.GetAllocator(), newCapacity);
        }
        else
        {
            ASSERT(m_storage.data != nullptr);
            m_storage.data = Memory::Reallocate<Type>(m_storage.GetAllocator(), m_storage.data, newCapacity, m_storage.capacity);
        }

        ASSERT_SLOW(m_storage.data != nullptr);
        m_storage.capacity = newCapacity;
    }
};

static_assert(sizeof(Array<u8>) == 24);
static_assert(sizeof(Array<u32>) == 24);
static_assert(sizeof(Array<u64>) == 24);
