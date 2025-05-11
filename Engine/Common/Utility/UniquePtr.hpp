#pragma once

#include "Memory/Memory.hpp"
#include "Memory/Allocators/Default.hpp"

template<typename Type, typename Deleter = std::conditional_t<std::is_void_v<Type>,
    Memory::VoidDeleter, Memory::AllocationDeleter<Type, Memory::Allocators::Default>>>
class UniquePtr final
{
    template<typename OtherType, typename OtherDeleter>
    friend class UniquePtr;

    struct DeleterInvoker
    {
        Deleter deleter = nullptr;

        DeleterInvoker(Deleter deleter)
            : deleter(deleter)
        {}

        void operator()(Type* pointer) const
        {
            if(deleter)
            {
                (*deleter)(pointer);
            }
        }
    };

    using DeleterType = std::conditional_t<std::is_pointer_v<Deleter>, DeleterInvoker, Deleter>;
    using StorageType = std::tuple<Type*, DeleterType>;

    StorageType m_storage;

public:
    UniquePtr()
        : m_storage(nullptr, Deleter{})
    {}

    UniquePtr(Type* pointer)
        : m_storage(pointer, Deleter{})
    {
        static_assert(!std::is_pointer_v<Deleter>, "Deleter function pointer must be provided");
    }

    UniquePtr(Type* pointer, Deleter&& deleter)
        : m_storage(pointer, Move(deleter))
    {
        EnsureDeleterPointer();
    }

    ~UniquePtr()
    {
        Delete();
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    template<typename OtherType, typename OtherDeleter>
    UniquePtr(UniquePtr<OtherType, OtherDeleter>&& other) noexcept
    {
        *this = Move(other);
    }

    template<typename OtherType, typename OtherDeleter>
    UniquePtr& operator=(UniquePtr<OtherType, OtherDeleter>&& other) noexcept
    {
        Reset();
        static_assert(std::is_convertible_v<OtherType*, Type*>, "Incompatible types");
        std::get<Type*>(m_storage) = std::get<OtherType*>(other.m_storage);
        std::get<DeleterType>(m_storage) = Move(std::get<OtherDeleter>(other.m_storage));
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

    void Reset()
    {
        Delete();
        m_storage = StorageType{nullptr, Deleter{}};
    }

    void Reset(Type* newPointer)
    {
        Delete();
        m_storage = StorageType{newPointer, Deleter{}};
        static_assert(!std::is_pointer_v<Deleter>, "Deleter function pointer must be provided");
    }

    void Reset(Type* newPointer, Deleter&& newDeleter)
    {
        Delete();
        m_storage = StorageType{newPointer, Move(newDeleter)};
        EnsureDeleterPointer();
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

private:
    void Delete()
    {
        if (Type* oldPointer = std::get<Type*>(m_storage))
        {
            DeleterType& oldDeleter = std::get<DeleterType>(m_storage);
            oldDeleter(oldPointer);
        }
    }

    void EnsureDeleterPointer() const
    {
        if constexpr(std::is_pointer_v<Deleter>)
        {
            ASSERT(std::get<DeleterInvoker>(m_storage).deleter,
                "Deleter function pointer must be valid");
        }
    }
};

using ErasedUniquePtr = UniquePtr<void>;

#ifndef COMPILER_MSVC
    static_assert(sizeof(UniquePtr<u8>) == 8);
    static_assert(sizeof(UniquePtr<u32>) == 8);
    static_assert(sizeof(UniquePtr<u64>) == 8);
    static_assert(sizeof(ErasedUniquePtr) == 16);
#else
    static_assert(sizeof(UniquePtr<u8>) == 16);
    static_assert(sizeof(UniquePtr<u32>) == 16);
    static_assert(sizeof(UniquePtr<u64>) == 16);
    static_assert(sizeof(ErasedUniquePtr) == 16);
#endif