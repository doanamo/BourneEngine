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

    template<typename Type, typename... Arguments>
    void Construct(Type* object, Arguments&&... arguments)
    {
        new (object) Type(std::forward<Arguments>(arguments)...);
    }

    template<typename Type, typename... Arguments>
    void ConstructRange(Type* begin, Type* end, Arguments&&... arguments)
    {
        ASSERT(begin <= end);
        for(Type* it = begin; it != end; ++it)
        {
            new (it) Type(arguments...);
        }
    }

    template<typename Type>
    void Destruct(Type* object)
    {
        object->~Type();
    }

    template<typename Type>
    void DestructRange(Type* begin, Type* end)
    {
        ASSERT(begin <= end);
        for(Type* it = begin; it != end; ++it)
        {
            it->~Type();
        }
    }

    template<typename Type, typename Allocator = DefaultAllocator, typename... Arguments>
    Type* New(Arguments&&... arguments)
    {
        return new (Allocate<Type, Allocator>()) Type(std::forward<Arguments>(arguments)...);
    }

    template<typename Type, typename Allocator = DefaultAllocator>
    void Delete(Type* object)
    {
        ASSERT(object);
        object->~Type();
        Deallocate<Type, Allocator>(object);
    }

    template<typename Type, typename Allocator = DefaultAllocator>
    struct Deleter
    {
        void operator()(Type* object)
        {
            Delete<Type, Allocator>(object);
        }
    };
}
