#pragma once

#include "Memory/Memory.hpp"

namespace Memory
{
    class DefaultAllocator final
    {
    public:
        DefaultAllocator() = delete;

        static void* Allocate(u64 size, u32 alignment);
        static void* Reallocate(void* allocation, u64 requestedSize, u64 previousSize, u32 alignment);
        static void Deallocate(void* allocation, u64 size, u32 alignment);

        template<typename ElementType>
        class TypedAllocation
        {
        private:
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

            void Allocate(u64 capacity)
            {
                ASSERT(m_pointer == nullptr);
                ASSERT_SLOW(m_capacity == 0);
                m_pointer = Memory::Allocate<ElementType, DefaultAllocator>(capacity);
                ASSERT_SLOW(m_pointer != nullptr);
                m_capacity = capacity;
            }

            void Reallocate(u64 capacity)
            {
                ASSERT(m_pointer != nullptr);
                ASSERT_SLOW(m_capacity != 0);
                m_pointer = Memory::Reallocate<ElementType, DefaultAllocator>(m_pointer, capacity, m_capacity);
                ASSERT_SLOW(m_pointer != nullptr);
                m_capacity = capacity;
            }

            void Deallocate()
            {
                ASSERT(m_pointer != nullptr);
                ASSERT_SLOW(m_capacity != 0);
                Memory::Deallocate<ElementType, DefaultAllocator>(m_pointer, m_capacity);
                m_pointer = nullptr;
                m_capacity = 0;
            }

            void Resize(u64 capacity)
            {
                if(m_capacity != 0)
                {
                    if(capacity == 0)
                    {
                        Deallocate();
                    }
                    else
                    {
                        Reallocate(capacity);
                    }
                }
                else
                {
                    Allocate(capacity);
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
