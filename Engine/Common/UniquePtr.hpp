#pragma once

#include "Memory/Allocator.hpp"

template<typename Type, auto Deleter = Memory::Deleter<Type>>
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
            Deleter(m_pointer);
        }
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept
    {
        *this = std::move(other);
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        ASSERT(this != &other);
        std::swap(m_pointer, other.m_pointer);
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

    Type* Get()
    {
        return m_pointer;
    }

    const Type* Get() const
    {
        return m_pointer;
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
};

template<typename Type, typename Allocator = DefaultAllocator, typename... Arguments>
auto MakeUnique(Arguments&&... arguments)
{
    return UniquePtr<Type, Memory::Deleter<Type, Allocator>>(
        new (Memory::Allocate<Type, Allocator>()) Type(std::forward<Arguments>(arguments)...));
}
