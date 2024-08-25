#pragma once

#include "Memory/Memory.hpp"

template<typename Type, typename Deleter = Memory::AllocationDeleter<Type, Memory::DefaultAllocator>>
class UniquePtr final
{
private:
    // Use empty base class optimization to avoid deleter size cost if it is stateless.
    struct Storage : public Deleter
    {
        Type* m_pointer = nullptr;

        Storage(Type* pointer = nullptr)
            : m_pointer(pointer)
        {
        }

        void Delete()
        {
            (*this)(m_pointer);
        }
    };

    Storage m_storage;

public:
    UniquePtr() = default;
    UniquePtr(Type* pointer)
        : m_storage(pointer)
    {
    }

    ~UniquePtr()
    {
        if(m_storage.m_pointer)
        {
            m_storage.Delete();
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
        return m_storage.m_pointer != nullptr;
    }

    bool operator==(const UniquePtr& other) const
    {
        return m_storage.m_pointer == other.m_storage.m_pointer;
    }

    bool operator!=(const UniquePtr& other) const
    {
        return m_storage.m_pointer != other.m_storage.m_pointer;
    }

    bool operator==(const Type* pointer) const
    {
        return m_storage.m_pointer == pointer;
    }

    bool operator!=(const Type* pointer) const
    {
        return m_storage.m_pointer != pointer;
    }

    Type* operator->()
    {
        ASSERT(m_storage.m_pointer);
        return m_storage.m_pointer;
    }

    Type& operator*()
    {
        ASSERT(m_storage.m_pointer);
        return *m_storage.m_pointer;
    }

    const Type* operator->() const
    {
        ASSERT(m_storage.m_pointer);
        return m_storage.m_pointer;
    }

    const Type& operator*() const
    {
        ASSERT(m_storage.m_pointer);
        return *m_storage.m_pointer;
    }

    Type* Get()
    {
        return m_storage.m_pointer;
    }

    const Type* Get() const
    {
        return m_storage.m_pointer;
    }

    void Reset(Type* pointer = nullptr)
    {
        if (m_storage.m_pointer)
        {
            m_storage.Delete();
        }

        m_storage.m_pointer = pointer;
    }

    Type* Detach()
    {
        Type* pointer = m_storage.m_pointer;
        m_storage.m_pointer = nullptr;
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
