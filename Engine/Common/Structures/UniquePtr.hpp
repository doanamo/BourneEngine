#pragma once

#include "Memory/Memory.hpp"
#include "Memory/Allocators/DefaultAllocator.hpp"

template<typename Type, typename Deleter = std::conditional_t<std::is_void_v<Type>,
    Memory::VoidDeleter, Memory::AllocationDeleter<Type, Memory::DefaultAllocator>>>
class UniquePtr final
{
    template<typename OtherType, typename OtherDeleter>
    friend class UniquePtr;

    struct DeleterInvoker
    {
        Deleter m_deleter = nullptr;

        DeleterInvoker(Deleter deleter)
            : m_deleter(deleter)
        {
        }

        void operator()(Type* pointer) const
        {
            if(m_deleter)
            {
                (*m_deleter)(pointer);
            }
        }
    };

    using DeleterType = std::conditional_t<std::is_pointer_v<Deleter>, DeleterInvoker, Deleter>;
    using StorageType = std::tuple<Type*, DeleterType>;

    StorageType m_storage;

public:
    explicit UniquePtr(Type* pointer = nullptr, Deleter&& deleter = {})
        : m_storage(pointer, std::move(deleter))
    {
    }

    ~UniquePtr()
    {
        if(Type* pointer = std::get<Type*>(m_storage))
        {
            DeleterType& deleter = std::get<DeleterType>(m_storage);
            deleter(pointer);
        }
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    template<typename OtherType, typename OtherDeleter>
    UniquePtr(UniquePtr<OtherType, OtherDeleter>&& other) noexcept
    {
        *this = std::move(other);
    }

    template<typename OtherType, typename OtherDeleter>
    UniquePtr& operator=(UniquePtr<OtherType, OtherDeleter>&& other) noexcept
    {
        Reset();
        static_assert(std::is_convertible_v<OtherType*, Type*>, "Incompatible types!");
        std::get<Type*>(m_storage) = std::get<OtherType*>(other.m_storage);
        std::get<DeleterType>(m_storage) = std::move(std::get<OtherDeleter>(other.m_storage));
        other.m_storage = {};
        return *this;
    }

    UniquePtr& operator=(std::nullptr_t)
    {
        Reset();
        return *this;
    }

    explicit operator bool() const
    {
        return IsValid();
    }

    bool operator==(const UniquePtr& other) const
    {
        return Get() == other.Get();
    }

    bool operator!=(const UniquePtr& other) const
    {
        return Get() != other.Get();
    }

    bool operator==(const Type* pointer) const
    {
        return Get() == pointer;
    }

    bool operator!=(const Type* pointer) const
    {
        return Get() != pointer;
    }

    Type* operator->()
    {
        ASSERT(IsValid());
        return Get();
    }

    auto& operator*()
    {
        ASSERT(IsValid());
        return *Get();
    }

    const Type* operator->() const
    {
        ASSERT(IsValid());
        return Get();
    }

    const auto& operator*() const
    {
        ASSERT(IsValid());
        return *Get();
    }

    Type* Get()
    {
        return std::get<Type*>(m_storage);
    }

    const Type* Get() const
    {
        return std::get<Type*>(m_storage);
    }

    void Reset(Type* newPointer = nullptr, DeleterType&& newDeleter = {})
    {
        if (Type* oldPointer = std::get<Type*>(m_storage))
        {
            Deleter& oldDeleter = std::get<DeleterType>(m_storage);
            oldDeleter(oldPointer);
        }

        m_storage = StorageType{newPointer, std::move(newDeleter)};
    }

    Type* Detach()
    {
        Type* pointer = std::get<Type*>(m_storage);
        m_storage = StorageType(nullptr, {});
        return pointer;
    }

    bool IsValid() const
    {
        return std::get<Type*>(m_storage) != nullptr;
    }
};

template<typename Type, typename Allocator = Memory::DefaultAllocator, typename... Arguments>
auto AllocateUnique(Arguments&&... arguments)
{
    return UniquePtr<Type, Memory::AllocationDeleter<Type, Allocator>>(
        Memory::New<Type, Allocator>(std::forward<Arguments>(arguments)...));
}

#ifndef COMPILER_MSVC
    static_assert(sizeof(UniquePtr<u8>) == 8);
    static_assert(sizeof(UniquePtr<u32>) == 8);
    static_assert(sizeof(UniquePtr<u64>) == 8);
#else
    // #todo: Implement custom version of std::tuple to have consistency across all compilers.
    static_assert(sizeof(UniquePtr<u8>) == 16);
    static_assert(sizeof(UniquePtr<u32>) == 16);
    static_assert(sizeof(UniquePtr<u64>) == 16);
#endif