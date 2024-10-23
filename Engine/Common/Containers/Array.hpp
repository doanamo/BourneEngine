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
        EnsureCapacity(other.m_size, CapacityMode::GrowExact);

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
        EnsureCapacity(m_size, CapacityMode::ForceExact);
    }

    void Reserve(u64 newCapacity)
    {
        EnsureCapacity(newCapacity, CapacityMode::GrowExact);
    }

    template<typename... Arguments>
    void Resize(u64 newSize, Arguments... arguments)
    {
        if(newSize > m_size) // Grow size
        {
            EnsureCapacity(newSize, CapacityMode::GrowExact);

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
    void Add(Arguments&&... arguments)
    {
        u64 newSize = m_size + 1;
        EnsureCapacity(newSize, CapacityMode::Grow);

        Memory::Construct(m_allocation.GetPointer() + m_size,
            std::forward<Arguments>(arguments)...);

        m_size = newSize;
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
        return Max(4ull, NextPow2(newCapacity - 1ull));
    }

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

static_assert(sizeof(Array<u8>) == 24);
static_assert(sizeof(Array<u32>) == 24);
static_assert(sizeof(Array<u64>) == 24);

template<typename ElementType, u64 ElementCount>
using InlineArray = Array<ElementType, Memory::InlineAllocator<ElementCount>>;
