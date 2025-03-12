#pragma once

#include "Memory/Memory.hpp"
#include "Memory/Allocators/Default.hpp"
#include "Memory/Allocators/Inline.hpp"

// Array container that stores elements in contiguous
// memory buffer that can be resized to fit more elements.
template<typename Type, typename Allocator = Memory::Allocators::Default>
class Array final
{
    using Allocation = typename Allocator::template TypedAllocation<Type>;
    Allocation m_allocation;
    u64 m_size = 0;

public:
    Array() = default;
    ~Array()
    {
        Clear();
    }

    Array(std::initializer_list<Type> elements)
    {
        Reserve(elements.size());
        for(const Type& element : elements)
        {
            Add(element);
        }
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
        Reserve(other.m_size);
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
        m_allocation.Resize(m_size, m_size);
    }

    void Reserve(const u64 capacity, const bool exact = true)
    {
        if(capacity > m_allocation.GetCapacity())
        {
            const u64 newCapacity = exact ? capacity : CalculateCapacity(capacity);
            m_allocation.Resize(newCapacity, m_size);
        }
    }

    template<typename... Arguments>
    void Resize(const u64 newSize, Arguments... arguments)
    {
        if(newSize > m_size) // Grow size
        {
            Reserve(newSize);
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
        Reserve(newSize, false);

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

    bool Contains(const Type& element) const
    {
        for(const Type* it = GetBegin(); it != GetEnd(); ++it)
        {
            if(*it == element)
                return true;
        }

        return false;
    }

    template<typename Predicate>
    bool ContainsPredicate(Predicate predicate) const
    {
        for(const Type* it = GetBegin(); it != GetEnd(); ++it)
        {
            if(predicate(*it))
                return true;
        }

        return false;
    }

    Type* Find(const Type& element)
    {
        for(Type* it = GetBegin(); it != GetEnd(); ++it)
        {
            if(*it == element)
                return it;
        }

        return nullptr;
    }

    template<typename Predicate>
    Type* FindPredicate(const Predicate& predicate)
    {
        for(Type* it = GetBegin(); it != GetEnd(); ++it)
        {
            if(predicate(*it))
                return it;
        }

        return nullptr;
    }

    const Type* Find(const Type& element) const
    {
        return const_cast<Type*>(std::as_const(*this).Find(element));
    }

    template<typename Predicate>
    const Type* FindPredicate(const Predicate& predicate) const
    {
        return const_cast<Type*>(std::as_const(*this).FindPredicate(predicate));
    }

    Type& operator[](const u64 index)
    {
        ASSERT(index < m_size, "Out of bounds access with %llu index and %llu size", index, m_size);
        return m_allocation.GetPointer()[index];
    }

    const Type& operator[](const u64 index) const
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

    bool IsEmpty() const
    {
        return m_size == 0;
    }

    Type* GetBegin()
    {
        return m_allocation.GetPointer();
    }

    Type* GetEnd()
    {
        return m_allocation.GetPointer() + m_size;
    }

    const Type* GetBegin() const
    {
        return m_allocation.GetPointer();
    }

    const Type* GetEnd() const
    {
        return m_allocation.GetPointer() + m_size;
    }

    Type* begin()
    {
        return GetBegin();
    }

    Type* end()
    {
        return GetEnd();
    }

    const Type* begin() const
    {
        return GetBegin();
    }

    const Type* end() const
    {
        return GetEnd();
    }

private:
    static u64 CalculateCapacity(const u64 newCapacity)
    {
        ASSERT(newCapacity != 0);

        // Find the next power of two capacity (unless already power of two),
        // but not smaller than some predefined minimum starting capacity.
        return std::max(4ull, NextPow2(newCapacity - 1ull));
    }
};

static_assert(sizeof(Array<u8>) == 24);
static_assert(sizeof(Array<u32>) == 24);
static_assert(sizeof(Array<u64>) == 24);

template<typename ElementType, u64 ElementCount>
using InlineArray = Array<ElementType, Memory::Allocators::Inline<ElementCount>>;

template<typename ElementType>
using HeapArray = Array<ElementType, Memory::Allocators::Default>;
