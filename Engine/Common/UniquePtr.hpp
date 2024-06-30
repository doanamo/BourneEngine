#pragma once

#include "Memory/Allocator.hpp"

template<typename Type, auto Deleter = Memory::Deleter<Type>>
class UniquePtr final
{
private:
    Type* m_pointer = nullptr;

public:
    UniquePtr() noexcept = default;
    explicit UniquePtr(Type* pointer) noexcept
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
        return m_pointer;
    }

    Type& operator*()
    {
        return *m_pointer;
    }

    const Type* operator->() const
    {
        return m_pointer;
    }

    const Type& operator*() const
    {
        return *m_pointer;
    }

    explicit operator bool() const
    {
        return m_pointer != nullptr;
    }
};

template<typename Type, typename Allocator = DefaultAllocator, typename... Arguments>
auto MakeUnique(Arguments&&... arguments)
{
    return UniquePtr<Type, Memory::Deleter<Type, Allocator>>(
        new (Memory::Allocate<Type, Allocator>()) Type(std::forward<Arguments>(arguments)...));
}
