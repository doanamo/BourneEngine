#pragma once

#include "Memory/Memory.hpp"
#include "Memory/Allocators/Default.hpp"
#include "Memory/Allocators/Inline.hpp"

// Array container that stores elements in a contiguous
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
        // Initializer lists in C++ were introduced before move semantics, so they do not allow moving, which results in unnecessary copies.
        static_assert(std::is_trivially_copyable_v<Type>, "Array initializer list elements must be trivially copyable due to performance reasons");

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
        *this = Move(other);
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
        m_allocation = Move(other.m_allocation);
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
                Forward<Arguments>(arguments)...);
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
        Memory::Construct(newElement, Forward<Arguments>(arguments)...);

        m_size = newSize;
        return *newElement;
    }

    template<typename Argument>
    Type& AddUnique(Argument&& element)
    {
        if(Type* existing = Find(element))
            return *existing;

        return Add(Forward<Argument>(element));
    }

    // #todo: Add remove function, with parameter for either moving last element into gap (default) or shifting all elements.
 
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

    // #todo: All single element Contain() and Find() methods should be outside functions that operate on iterators.

    bool Contains(const Type& element) const
    {
        for(const Type* it = GetBeginPtr(); it != GetEndPtr(); ++it)
        {
            if(*it == element)
                return true;
        }

        return false;
    }

    template<typename Predicate>
    bool ContainsPredicate(Predicate predicate) const
    {
        for(const Type* it = GetBeginPtr(); it != GetEndPtr(); ++it)
        {
            if(predicate(*it))
                return true;
        }

        return false;
    }

    Type* Find(const Type& element)
    {
        return const_cast<Type*>(std::as_const(*this).Find(element));
    }

    const Type* Find(const Type& element) const
    {
        for(const Type* it = GetBeginPtr(); it != GetEndPtr(); ++it)
        {
            if(*it == element)
                return it;
        }

        return nullptr;
    }

    template<typename Predicate>
    Type* FindPredicate(const Predicate& predicate)
    {
        return const_cast<Type*>(std::as_const(*this).FindPredicate(predicate));

    }

    template<typename Predicate>
    const Type* FindPredicate(const Predicate& predicate) const
    {
        for(const Type* it = GetBeginPtr(); it != GetEndPtr(); ++it)
        {
            if(predicate(*it))
                return it;
        }

        return nullptr;
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

    Type* GetBeginPtr()
    {
        return m_allocation.GetPointer();
    }

    Type* GetEndPtr()
    {
        return m_allocation.GetPointer() + m_size;
    }

    const Type* GetBeginPtr() const
    {
        return m_allocation.GetPointer();
    }

    const Type* GetEndPtr() const
    {
        return m_allocation.GetPointer() + m_size;
    }

    Type* begin()
    {
        return GetBeginPtr();
    }

    Type* end()
    {
        return GetEndPtr();
    }

    const Type* begin() const
    {
        return GetBeginPtr();
    }

    const Type* end() const
    {
        return GetEndPtr();
    }

private:
    static u64 CalculateCapacity(const u64 newCapacity)
    {
        ASSERT(newCapacity != 0);

        // Find the next power of two capacities (unless already power of two),
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
