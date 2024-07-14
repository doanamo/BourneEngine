#pragma once

#include "Memory/Allocator.hpp"

template<typename Type, typename Deleter = Memory::Deleter<Type>>
class UniquePtr final
{
private:
    Type* m_pointer = nullptr;

public:
    UniquePtr() noexcept = default;
    UniquePtr(Type* pointer) noexcept
        : m_pointer(pointer)
    {
    }

    ~UniquePtr()
    {
        if(m_pointer)
        {
            Deleter deleter;
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
        Reset(other.Release());
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
            Deleter deleter;
            deleter(m_pointer);
        }

        m_pointer = pointer;
    }

    Type* Release()
    {
        Type* pointer = m_pointer;
        m_pointer = nullptr;
        return pointer;
    }
};

template<typename Type, typename Allocator = Memory::DefaultAllocator, typename... Arguments>
auto MakeUnique(Arguments&&... arguments)
{
    return UniquePtr<Type, Memory::Deleter<Type, Allocator>>(
        new (Memory::Allocate<Type, Allocator>()) Type(std::forward<Arguments>(arguments)...));
}
