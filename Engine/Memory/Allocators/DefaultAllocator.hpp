#pragma once

#include "Memory/Memory.hpp"

namespace Memory
{
    class DefaultAllocator final
    {
    public:
        DefaultAllocator() = delete;

        static void* Allocate(u64 size, u32 alignment);
        static void* Reallocate(void* allocation, u64 newSize, u64 oldSize, u32 alignment);
        static void Deallocate(void* allocation, u64 size, u32 alignment);

        template<typename ElementType>
        class TypedAllocation
        {
            ElementType* m_pointer = nullptr;
            u64 m_capacity = 0;

        public:
            TypedAllocation() = default;
            ~TypedAllocation()
            {
                if(m_pointer)
                {
                    Deallocate();
                }
            }

            TypedAllocation(const TypedAllocation&) = delete;
            TypedAllocation& operator=(const TypedAllocation&) = delete;

            TypedAllocation(TypedAllocation&& other) noexcept
            {
                *this = std::move(other);
            }

            TypedAllocation& operator=(TypedAllocation&& other) noexcept
            {
                ASSERT_SLOW(this != &other);

                if(m_capacity != 0)
                {
                    Deallocate();
                }

                m_pointer = other.m_pointer;
                other.m_pointer = nullptr;

                m_capacity = other.m_capacity;
                other.m_capacity = 0;

                return *this;
            }

            void Allocate(const u64 capacity)
            {
                ASSERT(m_pointer == nullptr);
                ASSERT_SLOW(m_capacity == 0);
                m_pointer = Memory::Allocate<ElementType, DefaultAllocator>(capacity);
                ASSERT_SLOW(m_pointer != nullptr);
                m_capacity = capacity;
            }

            void Reallocate(const u64 newCapacity, const u64 usedCapacity)
            {
                ASSERT(m_pointer != nullptr);
                ASSERT_SLOW(m_capacity != 0);
                m_pointer = Memory::Reallocate<ElementType, DefaultAllocator>(m_pointer, newCapacity, m_capacity);
                ASSERT_SLOW(m_pointer != nullptr);
                m_capacity = newCapacity;
            }

            void Deallocate()
            {
                ASSERT(m_pointer != nullptr);
                ASSERT_SLOW(m_capacity != 0);
                Memory::Deallocate<ElementType, DefaultAllocator>(m_pointer, m_capacity);
                m_pointer = nullptr;
                m_capacity = 0;
            }

            void Resize(const u64 newCapacity, const u64 usedCapacity)
            {
                if(m_capacity != 0)
                {
                    if(newCapacity == 0)
                    {
                        Deallocate();
                    }
                    else
                    {
                        Reallocate(newCapacity, usedCapacity);
                    }
                }
                else
                {
                    Allocate(newCapacity);
                }
            }

            ElementType* GetPointer() const
            {
                return m_pointer;
            }

            u64 GetCapacity() const
            {
                return m_capacity;
            }
        };
    };
}
