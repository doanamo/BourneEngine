#pragma once

#include "Utility.hpp"

namespace Memory
{
    namespace Allocators
    {
        class Default;
    }

    constexpr u64 UnknownCount = 0;
    constexpr u64 UnknownSize = 0;
    constexpr u64 UnknownAlignment = 0;

    template<typename Type, typename Allocator = Allocators::Default>
    Type* Allocate(const u64 count = 1)
    {
        return static_cast<Type*>(Allocator::Allocate(sizeof(Type) * count, alignof(Type)));
    }

    template<typename Type, typename Allocator = Allocators::Default>
    Type* Reallocate(Type* allocation, const u64 requestedCount, const u64 currentCount = UnknownCount)
    {
        return static_cast<Type*>(Allocator::Reallocate(allocation,
            sizeof(Type) * requestedCount, sizeof(Type) * currentCount, alignof(Type)));
    }

    template<typename Type, typename Allocator = Allocators::Default>
    void Deallocate(Type* allocation, const u64 count = UnknownCount)
    {
        Allocator::Deallocate(allocation, sizeof(Type) * count, alignof(Type));
    }

    template<typename Type, typename... Arguments>
    void Construct(Type* object, Arguments&&... arguments)
    {
        new (object) Type(Forward<Arguments>(arguments)...);
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
    void CopyConstructRange(Type* destination, const Type* source, const u64 count)
    {
        ASSERT(destination != nullptr);
        ASSERT(source != nullptr);

        if constexpr(std::is_trivially_copyable<Type>())
        {
            std::memcpy(destination, source, sizeof(Type) * count);
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
    void MoveConstructRange(Type* destination, Type* source, const u64 count)
    {
        ASSERT(destination != nullptr);
        ASSERT(source != nullptr);

        if constexpr(std::is_trivially_copyable<Type>())
        {
            std::memcpy(destination, source, sizeof(Type) * count);
        }
        else
        {
            for(u64 i = 0; i < count; ++i)
            {
                new (destination + i) Type(Move(source[i]));
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

        MarkDestructed(object, sizeof(Type));
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

        MarkDestructed(begin, sizeof(Type) * (end - begin));
    }

    template<typename Type, typename Allocator = Allocators::Default, typename... Arguments>
    Type* New(Arguments&&... arguments)
    {
        Type* object = Allocate<Type, Allocator>();
        Construct<Type>(object, Forward<Arguments>(arguments)...);
        return object;
    }

    template<typename Type, typename Allocator = Allocators::Default>
    void Delete(Type* object)
    {
        if(object)
        {
            Destruct<Type>(object);
            Deallocate<Type, Allocator>(object);
        }
    }

    using VoidDeleter = void(*)(void*);

    template<typename Type, typename Allocator>
    struct AllocationDeleter
    {
        void operator()(Type* object)
        {
            Delete<Type, Allocator>(object);
        }

        template<typename OtherType>
        AllocationDeleter& operator=(AllocationDeleter<OtherType, Allocator>&& other) noexcept
        {
            // Allow conversion between both allocators if they are of convertible types.
            static_assert(std::is_convertible_v<OtherType*, Type*>, "Incompatible types");
            return *this;
        }
    };
}
