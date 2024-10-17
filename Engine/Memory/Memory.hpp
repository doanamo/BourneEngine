#pragma once

namespace Memory
{
    constexpr u8 UninitializedPattern = 0xBE;
    constexpr u8 FreedPattern = 0xFE;

    inline void FillUninitializedPattern(void* memory, u64 size)
    {
#ifdef ENABLE_MEMORY_FILL
        std::memset(memory, UninitializedPattern, size);
#endif
    }

    inline void FillFreedPattern(void* memory, u64 size)
    {
#ifdef ENABLE_MEMORY_FILL
        std::memset(memory, FreedPattern, size);
#endif
    }

    constexpr u64 UnknownCount = 0;
    constexpr u64 UnknownSize = 0;

    template<typename Type, typename Allocator = DefaultAllocator>
    Type* Allocate(u64 count = 1)
    {
        return (Type*)Allocator::Allocate(sizeof(Type) * count, alignof(Type));
    }

    template<typename Type, typename Allocator = DefaultAllocator>
    Type* Reallocate(Type* allocation, u64 requestedCount, u64 currentCount = UnknownCount)
    {
        return (Type*)Allocator::Reallocate(allocation, sizeof(Type) * requestedCount, sizeof(Type) * currentCount, alignof(Type));
    }

    template<typename Type, typename Allocator = DefaultAllocator>
    void Deallocate(Type* allocation, u64 count = UnknownCount)
    {
        Allocator::Deallocate(allocation, sizeof(Type) * count, alignof(Type));
    }

    template<typename Type, typename Allocator = DefaultAllocator, typename... Arguments>
    Type* New(Arguments&&... arguments)
    {
        Type* object = Allocate<Type, Allocator>();
        Construct<Type>(object, std::forward<Arguments>(arguments)...);
        return object;
    }

    template<typename Type, typename Allocator = DefaultAllocator>
    void Delete(Type* object)
    {
        ASSERT(object != nullptr);
        Destruct<Type>(object);
        Deallocate<Type, Allocator>(object);
    }

    template<typename Type, typename... Arguments>
    void Construct(Type* object, Arguments&&... arguments)
    {
        new (object) Type(std::forward<Arguments>(arguments)...);
    }

    template<typename Type, typename... Arguments>
    void ConstructRange(Type* begin, Type* end, const Arguments&... arguments)
    {
        ASSERT(begin <= end);
        for(Type* it = begin; it != end; ++it)
        {
            Construct<Type>(it, arguments...);
        }
    }

    template<typename Type>
    void CopyConstructRange(Type* destination, const Type* source, u64 count)
    {
        ASSERT(destination != nullptr);
        ASSERT(source != nullptr);
        if constexpr(std::is_trivially_copyable<Type>())
        {
            memcpy(destination, source, sizeof(Type) * count);
        }
        else
        {
            for(u64 i = 0; i < count; ++i)
            {
                new (destination + i) Type(source[i]);
            }
        }
    }

    template<typename Type>
    void MoveConstructRange(Type* destination, Type* source, u64 count)
    {
        ASSERT(destination != nullptr);
        ASSERT(source != nullptr);
        if constexpr(std::is_trivially_copyable<Type>())
        {
            memcpy(destination, source, sizeof(Type) * count);
        }
        else
        {
            for(u64 i = 0; i < count; ++i)
            {
                new (destination + i) Type(std::move(source[i]));
            }
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
            for(Type* object = begin; object != end; ++object)
            {
                object->~Type();
            }
        }
        FillUninitializedPattern(begin, sizeof(Type) * (end - begin));
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
