#pragma once

#include "Allocators/DefaultAllocator.hpp"
#include "Allocators/InlineAllocator.hpp"

namespace Memory
{
    constexpr u8 UninitializedPattern = 0xBE;
    constexpr u8 FreedPattern = 0xFE;

    inline void FillUninitializedPattern(void* memory, u64 size)
    {
#ifdef ENABLE_MEMORY_FILL
        memset(memory, UninitializedPattern, size);
#endif
    }

    inline void FillFreedPattern(void* memory, u64 size)
    {
#ifdef ENABLE_MEMORY_FILL
        memset(memory, FreedPattern, size);
#endif
    }

    constexpr u64 UnknownCount = 0;
    constexpr u64 UnknownSize = 0;

    template<typename Type, typename Allocator = DefaultAllocator>
    Type* Allocate(u64 count = 1)
    {
        static_assert(Allocator::IsStatic);
        return (Type*)Allocator::Allocate(sizeof(Type) * count, alignof(Type));
    }

    template<typename Type, typename Allocator>
    Type* Allocate(Allocator& allocator, u64 count = 1)
    {
        return (Type*)allocator.Allocate(sizeof(Type) * count, alignof(Type));
    }

    template<typename Type, typename Allocator = DefaultAllocator>
    Type* Reallocate(Type* allocation, u64 requestedCount, u64 currentCount = UnknownCount)
    {
        static_assert(Allocator::IsStatic);
        return (Type*)Allocator::Reallocate(allocation, sizeof(Type) * requestedCount, sizeof(Type) * currentCount, alignof(Type));
    }

    template<typename Type, typename Allocator>
    Type* Reallocate(Allocator& allocator, Type* allocation, u64 requestedCount, u64 currentCount = UnknownCount)
    {
        return (Type*)allocator.Reallocate(allocation, sizeof(Type) * requestedCount, sizeof(Type) * currentCount, alignof(Type));
    }

    template<typename Type, typename Allocator = DefaultAllocator>
    void Deallocate(Type* allocation, u64 count = UnknownCount)
    {
        static_assert(Allocator::IsStatic);
        Allocator::Deallocate(allocation, sizeof(Type) * count, alignof(Type));
    }

    template<typename Type, typename Allocator>
    void Deallocate(Allocator& allocator, Type* allocation, u64 count = UnknownCount)
    {
       allocator.Deallocate(allocation, sizeof(Type) * count, alignof(Type));
    }

    template<typename Type, typename Allocator = DefaultAllocator, typename... Arguments>
    Type* New(Arguments&&... arguments)
    {
        static_assert(Allocator::IsStatic);
        return new (Allocate<Type, Allocator>()) Type(std::forward<Arguments>(arguments)...);
    }

    template<typename Type, typename Allocator, typename... Arguments>
    Type* New(Allocator& allocator, Arguments&&... arguments)
    {
        return new (Allocate<Type>(allocator)) Type(std::forward<Arguments>(arguments)...);
    }

    template<typename Type, typename Allocator = DefaultAllocator>
    void Delete(Type* object)
    {
        ASSERT(object != nullptr);
        Destruct<Type>(object);
        static_assert(Allocator::IsStatic);
        Deallocate<Type, Allocator>(object);
    }

    template<typename Type, typename Allocator>
    void Delete(Allocator& allocator, Type* object)
    {
        ASSERT(object != nullptr);
        Destruct<Type>(object);
        Deallocate<Type>(allocator, object);
    }

    template<typename Type, typename... Arguments>
    void Construct(Type* object, Arguments&&... arguments)
    {
        new (object) Type(std::forward<Arguments>(arguments)...);
    }

    template<typename Type, typename... Arguments>
    void ConstructRange(Type* begin, Type* end, Arguments&&... arguments)
    {
        // #optimize: It may be possible to use memset for ranges of trivially constructible types.
        ASSERT(begin <= end);
        for(Type* it = begin; it != end; ++it)
        {
            new (it) Type(arguments...);
        }
    }

    template<typename Type>
    void Destruct(Type* object)
    {
        if constexpr(!std::is_trivially_destructible<Type>())
        {
            object->~Type();
        }
        FillUninitializedPattern(object, sizeof(Type));
    }

    template<typename Type>
    void DestructRange(Type* begin, Type* end)
    {
        ASSERT(begin <= end);
        if constexpr(!std::is_trivially_destructible<Type>())
        {
            for(Type* it = begin; it != end; ++it)
            {
                it->~Type();
            }
        }
        FillUninitializedPattern(begin, (u8*)end - (u8*)begin);
    }

    template<typename Type, typename Allocator>
    struct AllocationDeleter
    {
    public:
        void operator()(Type* object)
        {
            Delete<Type, Allocator>(object);
        }
    };
}
