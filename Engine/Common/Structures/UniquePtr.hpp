#pragma once

#include "Memory/Memory.hpp"
#include "Memory/Allocators/DefaultAllocator.hpp"

template<typename Type, typename Deleter = Memory::AllocationDeleter<Type, Memory::DefaultAllocator>>
class UniquePtr final
{
private:
    struct Storage : public Deleter // Empty base class optimization.
    {
        Type* pointer = nullptr;

        Storage(Type* pointer = nullptr)
            : pointer(pointer)
        {
        }

        Deleter& GetDeleter()
        {
            return *this;
        }
    } m_storage;

public:
    UniquePtr() = default;
    UniquePtr(Type* pointer)
        : m_storage(pointer)
    {
    }

    ~UniquePtr()
    {
        if(m_storage.pointer)
        {
            m_storage.GetDeleter()(m_storage.pointer);
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
        return m_storage.pointer != nullptr;
    }

    bool operator==(const UniquePtr& other) const
    {
        return m_storage.pointer == other.m_storage.pointer;
    }

    bool operator!=(const UniquePtr& other) const
    {
        return m_storage.pointer != other.m_storage.pointer;
    }

    bool operator==(const Type* pointer) const
    {
        return m_storage.pointer == pointer;
    }

    bool operator!=(const Type* pointer) const
    {
        return m_storage.pointer != pointer;
    }

    Type* operator->()
    {
        ASSERT(m_storage.pointer);
        return m_storage.pointer;
    }

    Type& operator*()
    {
        ASSERT(m_storage.pointer);
        return *m_storage.pointer;
    }

    const Type* operator->() const
    {
        ASSERT(m_storage.pointer);
        return m_storage.pointer;
    }

    const Type& operator*() const
    {
        ASSERT(m_storage.pointer);
        return *m_storage.pointer;
    }

    Type* Get()
    {
        return m_storage.pointer;
    }

    const Type* Get() const
    {
        return m_storage.pointer;
    }

    void Reset(Type* pointer = nullptr)
    {
        if (m_storage.pointer)
        {
            m_storage.GetDeleter()(m_storage.pointer);
        }

        m_storage.pointer = pointer;
    }

    Type* Detach()
    {
        Type* pointer = m_storage.pointer;
        m_storage.pointer = nullptr;
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
