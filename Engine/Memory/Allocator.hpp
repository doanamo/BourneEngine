#pragma once

#include "DefaultAllocator.hpp"

namespace Memory
{
    template<typename Type, typename Allocator = DefaultAllocator>
    Type* Allocate(u64 count = 1)
    {
        return (Type*)Allocator::Allocate(sizeof(Type) * count, alignof(Type));
    }

    template<typename Type, typename Allocator = DefaultAllocator>
    Type* Reallocate(Type* allocation, u64 count)
    {
        return (Type*)Allocator::Reallocate(allocation, sizeof(Type) * count, alignof(Type));
    }

    template<typename Type, typename Allocator = DefaultAllocator>
    void Deallocate(Type* allocation)
    {
        Allocator::Deallocate(allocation, alignof(Type));
    }

    template<typename Type, typename Allocator = DefaultAllocator>
    void Deleter(Type* object)
    {
        object->~Type();
        Allocator::Deallocate(object, alignof(Type));
    }
}
