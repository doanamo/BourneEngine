#pragma once

#include "Memory/Memory.hpp"

template<typename Type, typename AllocationDeleter = Memory::AllocationDeleter<Type, Memory::DefaultAllocator>>
class UniquePtr final
{
private:
    Type* m_pointer = nullptr;

public:
    UniquePtr() = default;
    UniquePtr(Type* pointer)
        : m_pointer(pointer)
    {
    }

    ~UniquePtr()
    {
        if(m_pointer)
        {
            // #todo: Deleter should be stored in class (e.g. when deleter contains lambda with capture).
            // Employ empty base optimization so UniquePtr stays 8 bytes when possible.
            AllocationDeleter deleter;
            deleter(m_pointer);
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
        static_assert(std::is_convertible_v<OtherType*, Type*>, "Incompatible types!");
        Reset(other.Detach());
        return *this;
    }

    explicit operator bool() const
    {
        return m_pointer != nullptr;
    }

    bool operator==(const UniquePtr& other) const
    {
        return m_pointer == other.m_pointer;
    }

    bool operator!=(const UniquePtr& other) const
    {
        return m_pointer != other.m_pointer;
    }

    bool operator==(const Type* pointer) const
    {
        return m_pointer == pointer;
    }

    bool operator!=(const Type* pointer) const
    {
        return m_pointer != pointer;
    }

    Type* operator->()
    {
        ASSERT(m_pointer);
        return m_pointer;
    }

    Type& operator*()
    {
        ASSERT(m_pointer);
        return *m_pointer;
    }

    const Type* operator->() const
    {
        ASSERT(m_pointer);
        return m_pointer;
    }

    const Type& operator*() const
    {
        ASSERT(m_pointer);
        return *m_pointer;
    }

    Type* Get()
    {
        return m_pointer;
    }

    const Type* Get() const
    {
        return m_pointer;
    }

    void Reset(Type* pointer = nullptr)
    {
        if (m_pointer)
        {
            AllocationDeleter deleter;
            deleter(m_pointer);
        }

        m_pointer = pointer;
    }

    Type* Detach()
    {
        Type* pointer = m_pointer;
        m_pointer = nullptr;
        return pointer;
    }
};

template<typename Type, typename Allocator = Memory::DefaultAllocator, typename... Arguments>
auto AllocateUnique(Arguments&&... arguments)
{
    return UniquePtr<Type, Memory::AllocationDeleter<Type, Allocator>>(
        new (Memory::Allocate<Type, Allocator>()) Type(std::forward<Arguments>(arguments)...));
}

static_assert(sizeof(UniquePtr<u8>) == 8);
static_assert(sizeof(UniquePtr<u32>) == 8);
static_assert(sizeof(UniquePtr<u64>) == 8);
