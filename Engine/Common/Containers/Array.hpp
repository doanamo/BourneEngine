#pragma once

#include "Memory/Memory.hpp"

template<typename Type>
class Array final
{
private:
    Allocator* m_allocator = nullptr;
    Type* m_data = nullptr;
    u64 m_capacity = 0;
    u64 m_size = 0;

public:
    Array(Allocator& allocator = Memory::GetDefaultAllocator())
        : m_allocator(&allocator)
    {
        ASSERT(m_allocator);
    }

    ~Array()
    {
        if(m_data)
        {
            ASSERT(m_capacity > 0);
            ASSERT(m_size <= m_capacity);
            DestructElements(m_data, m_data + m_size);
            m_allocator->Deallocate(m_data);
        }
    }

    Array(const Array& other) = delete;
    Array& operator=(const Array& other) = delete;

    Array(Array&& other)
    {
        *this = std::move(other);
    }

    Array& operator=(Array&& other)
    {
        std::swap(m_allocator, other.m_allocator);
        std::swap(m_data, other.m_data);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_size, other.m_size);
        return *this;
    }

    void Reserve(u64 newCapacity)
    {
        if(newCapacity > m_capacity)
        {
            Allocate(newCapacity, true);
            ASSERT(m_capacity >= newCapacity);
        }
    }

    void Resize(u64 newSize, const Type& value = Type())
    {
        if(newSize > m_size) // Grow size
        {
            if(newSize > m_capacity) // Grow capacity
            {
                Allocate(newSize, true);
                ASSERT(m_capacity >= newSize);
            }

            ConstructElements(m_data + m_size, m_data + newSize, value);
        }
        else if(newSize < m_size) // Shrink size
        {
            DestructElements(m_data + newSize, m_data + m_size);
        }

        m_size = newSize;
    }

    void PushBack(const Type& value)
    {
        if(m_size == m_capacity)
        {
            Allocate(m_capacity + 1, false);
            ASSERT(m_size + 1 == m_capacity);
        }

        Type* newElement = m_data + m_size++;
        ConstructElements(newElement, newElement + 1, value);
    }

    void EmplaceBack(Type&& value)
    {
        if(m_size == m_capacity)
        {
            Allocate(m_capacity + 1, false);
            ASSERT(m_size + 1 == m_capacity);
        }

        Type* newElement = m_data + m_size++;
        ConstructElements(newElement, newElement + 1, std::forward<Type>(value));
    }
 
    void ShrinkToFit()
    {
        if(m_capacity > m_size)
        {
            Allocate(m_size, true);
            ASSERT(m_capacity == m_size);
        }
    }

    void Clear()
    {
        DestructElements(m_data, m_data + m_size);
        m_size = 0;
    }

    Type& operator[](u64 index)
    {
        ASSERT(index < m_size, "Out of bounds access with"
            " %llu index and %llu size", index, m_size);
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
    
    u64 GetSize() const
    {
        return m_size;
    }

    u64 GetSizeBytes() const
    {
        return GetSize() * sizeof(Type);
    }

    u64 GetCapacity() const
    {
        return m_capacity;
    }

    u64 GetCapacityBytes() const
    {
        return m_capacity * sizeof(Type);
    }

    u64 GetUnuseedCapacity() const
    {
        return m_capacity - m_size;
    }

    u64 GetUnusedCapacityBytes() const
    {
        return (m_capacity - m_size) * sizeof(Type);
    }

private:
    u64 CalculateGrowth(u64 newCapacity)
    {
        return NextPow2(newCapacity);
    }

    void Allocate(u64 newCapacity, bool exactCapacity)
    {
        ASSERT(newCapacity != m_capacity, "Must not be called with current capacity");

        if(!exactCapacity && newCapacity > m_capacity) // Grow capacity
        {
            newCapacity = CalculateGrowth(newCapacity);
            ASSERT(newCapacity > m_capacity);
        }

        if(m_capacity == 0) // Allocate buffer
        {
            ASSERT(m_data == nullptr);
            m_data = m_allocator->Allocate<Type>(newCapacity);
        }
        else if(newCapacity == 0) // Deallocate buffer
        {
            ASSERT(m_data != nullptr);
            DestructElements(m_data, m_data + m_size);
            m_allocator->Deallocate(m_data);
            m_data = nullptr;
        }
        else // Reallocate buffer
        {
            ASSERT(m_data != nullptr);
            if(newCapacity < m_capacity) // Shrink capacity
            {
                DestructElements(m_data + newCapacity, m_data + m_size);
            }

            m_data = m_allocator->Reallocate(m_data, newCapacity);
        }

        m_capacity = newCapacity;
    }

    template<typename ValueType>
    void ConstructElements(Type* begin, Type* end, ValueType value)
    {
        ASSERT(begin <= end);
        for(Type* it = begin; it != end; ++it)
        {
            new (it) Type(std::forward<ValueType>(value));
        }
    }

    void DestructElements(Type* begin, Type* end)
    {
        ASSERT(begin <= end);
        for(Type* it = begin; it != end; ++it)
        {
            it->~Type();
        }
    }
};
