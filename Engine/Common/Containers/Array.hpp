#pragma once

#include "Memory/Memory.hpp"
#include "Memory/Allocators/DefaultAllocator.hpp"
#include "Memory/Allocators/InlineAllocator.hpp"

// Array container that stores elements in contiguous
// memory buffer that can be resized to fit more elements.
template<typename Type, typename Allocator = Memory::DefaultAllocator>
class Array final
{
private:
    using Allocation = typename Allocator::template TypedAllocation<Type>;
    Allocation m_allocation;
    u64 m_size = 0;

public:
    Array() = default;
    ~Array()
    {
        Clear();
    }

    Array(const Array& other)
    {
        *this = other;
    }

    Array(Array&& other) noexcept
    {
        *this = std::move(other);
    }

    Array& operator=(const Array& other)
    {
        ASSERT_SLOW(this != &other);

        Clear();

        if(other.m_size > m_allocation.GetCapacity())
        {
            m_allocation.Resize(other.m_size);
        }

        Memory::CopyConstructRange(
            m_allocation.GetPointer(),
            other.m_allocation.GetPointer(),
            other.m_size);

        m_size = other.m_size;
        return *this;
    }

    Array& operator=(Array&& other) noexcept
    {
        ASSERT_SLOW(this != &other);
        m_allocation = std::move(other.m_allocation);
        m_size = other.m_size;
        other.m_size = 0;
        return *this;
    }

    void ShrinkToFit()
    {
        m_allocation.Resize(m_size);
    }

    void Reserve(u64 newCapacity)
    {
        if(newCapacity > m_allocation.GetCapacity())
        {
            m_allocation.Resize(newCapacity);
        }
    }

    template<typename... Arguments>
    void Resize(u64 newSize, Arguments... arguments)
    {
        if(newSize > m_size) // Grow size
        {
            if(newSize > m_allocation.GetCapacity())
            {
                m_allocation.Resize(newSize);
            }

            Memory::ConstructRange(
                m_allocation.GetPointer() + m_size,
                m_allocation.GetPointer() + newSize,
                std::forward<Arguments>(arguments)...);
        }
        else if(newSize < m_size) // Shrink size
        {
            Memory::DestructRange(
                m_allocation.GetPointer() + newSize,
                m_allocation.GetPointer() + m_size);
        }

        m_size = newSize;
    }

    template<typename... Arguments>
    Type& Add(Arguments&&... arguments)
    {
        const u64 newSize = m_size + 1;
        const u64 newCapacity = CalculateCapacity(newSize);
        if(newCapacity > m_allocation.GetCapacity())
        {
            m_allocation.Resize(newCapacity);
        }

        Type* newElement = m_allocation.GetPointer() + m_size;
        Memory::Construct(newElement, std::forward<Arguments>(arguments)...);

        m_size = newSize;
        return *newElement;
    }
 
    void Clear()
    {
        if(m_size > 0)
        {
            Memory::DestructRange(
                m_allocation.GetPointer(),
                m_allocation.GetPointer() + m_size);

            m_size = 0;
        }
    }

    Type& operator[](u64 index)
    {
        ASSERT(index < m_size, "Out of bounds access with %llu index and %llu size", index, m_size);
        return m_allocation.GetPointer()[index];
    }

    const Type& operator[](u64 index) const
    {
        ASSERT(index < m_size, "Out of bounds access with %llu index and %llu size", index, m_size);
        return m_allocation.GetPointer()[index];
    }

    Type* GetData()
    {
        return m_allocation.GetPointer();
    }

    const Type* GetData() const
    {
        return m_allocation.GetPointer();
    }
    
    u64 GetCapacity() const
    {
        return m_allocation.GetCapacity();
    }

    u64 GetCapacityBytes() const
    {
        return GetCapacity() * sizeof(Type);
    }

    u64 GetSize() const
    {
        return m_size;
    }

    u64 GetSizeBytes() const
    {
        return GetSize() * sizeof(Type);
    }

    u64 GetUnusedCapacity() const
    {
        ASSERT_SLOW(m_allocation.GetCapacity() >= m_size);
        return m_allocation.GetCapacity() - m_size;
    }

    u64 GetUnusedCapacityBytes() const
    {
        return GetUnusedCapacity() * sizeof(Type);
    }

private:
    u64 CalculateCapacity(u64 newCapacity)
    {
        // Find the next power of two capacity (unless already power of two),
        // but not smaller than some predefined minimum starting capacity.
        return Max(4ull, NextPow2(newCapacity - 1ull));
    }
};

static_assert(sizeof(Array<u8>) == 24);
static_assert(sizeof(Array<u32>) == 24);
static_assert(sizeof(Array<u64>) == 24);

template<typename ElementType, u64 ElementCount>
using InlineArray = Array<ElementType, Memory::InlineAllocator<ElementCount>>;
