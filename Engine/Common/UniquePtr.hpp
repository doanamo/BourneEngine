#pragma once

#include "Memory/Memory.hpp"

template<typename Type, typename Deleter = Memory::Deleter<Type, Memory::DefaultAllocator>>
class UniquePtr final
{
private:
    static_assert(std::is_same_v<Type, Deleter::DeletedType>, "Deleter type must match pointer type!");

    Deleter m_deleter;
    Type* m_pointer = nullptr;

public:
    UniquePtr(Deleter::AllocatorType& allocator)
        : m_deleter(allocator)
    {
    }

    UniquePtr(Deleter::AllocatorType& allocator, Type* pointer)
        : m_deleter(allocator)
        , m_pointer(pointer)
    {
    }

    UniquePtr(Deleter&& deleter) noexcept
        : m_deleter(std::move(deleter))
    {
    }

    UniquePtr(Deleter&& deleter, Type* pointer) noexcept
        : m_deleter(std::move(deleter))
        , m_pointer(pointer)
    {
    }

    ~UniquePtr()
    {
        if(m_pointer)
        {
            m_deleter(m_pointer);
        }
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    template<typename OtherType, typename OtherDeleter>
    UniquePtr(UniquePtr<OtherType, OtherDeleter>&& other) noexcept
        : m_deleter(other.GetDeleter())
    {
        *this = std::move(other);
    }

    template<typename OtherType, typename OtherDeleter>
    UniquePtr& operator=(UniquePtr<OtherType, OtherDeleter>&& other) noexcept
    {
        static_assert(std::is_convertible_v<OtherType*, Type*>, "Incompatible types!");
        ASSERT(m_deleter == other.GetDeleter(), "Deleters must match!");
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
            m_deleter(m_pointer);
        }

        m_pointer = pointer;
    }

    Type* Detach()
    {
        Type* pointer = m_pointer;
        m_pointer = nullptr;
        return pointer;
    }

    const Deleter& GetDeleter() const
    {
        return m_deleter;
    }
};

template<typename Type, typename Allocator, typename... Arguments>
auto MakeUnique(Allocator& allocator, Arguments&&... arguments)
{
    return UniquePtr<Type>(Memory::Deleter<Type, Allocator>(allocator), new (Memory::Allocate<Type>(allocator)) Type(std::forward<Arguments>(arguments)...));
}

static_assert(sizeof(UniquePtr<u8>) == 16);
static_assert(sizeof(UniquePtr<u32>) == 16);
static_assert(sizeof(UniquePtr<u64>) == 16);
