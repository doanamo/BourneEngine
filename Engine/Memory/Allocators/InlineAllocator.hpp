#pragma once

#include "Memory/Memory.hpp"
#include "Memory/MemoryStats.hpp"

namespace Memory
{
    template<u64 ElementCount, typename SecondaryAllocator = Memory::DefaultAllocator>
    class InlineAllocator final
    {
    public:
        InlineAllocator() = delete;

        template<typename ElementType>
        class TypedAllocation final
        {
        private:
            using SecondaryAllocation = typename SecondaryAllocator::template TypedAllocation<ElementType>;

            union Union
            {
                Union()
                {
                    // Implemented by outer class.
                }

                ~Union()
                {
                    // Implemented by outer class.
                }

                TypeStorage<ElementType> elements[ElementCount];
                SecondaryAllocation secondary;
            } m_union;

            u64 m_capacity = 0;

        public:
            TypedAllocation()
            {
                MarkUnitialized(&m_union, sizeof(m_union));
            }

            ~TypedAllocation()
            {
                if(m_capacity != 0)
                {
                    Deallocate();
                }

                MarkFreed(&m_union, sizeof(m_union));
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

                if(other.m_capacity == 0)
                {
                    return *this;
                }

                MarkUnitialized(&m_union, sizeof(m_union));

                if(IsInlineCapacity(other.m_capacity))
                {
                    std::memcpy(m_union.elements, other.m_union.elements, sizeof(ElementType) * other.m_capacity);
                    MarkFreed(&other.m_union.elements, sizeof(ElementType) * other.m_capacity);
                }
                else
                {
                    new (&m_union.secondary) SecondaryAllocation();
                    m_union.secondary = std::move(other.m_union.secondary);
                    other.m_union.secondary.~SecondaryAllocation();
                    MarkFreed(&other.m_union.secondary, sizeof(other.m_union.secondary));
                }

                m_capacity = other.m_capacity;
                other.m_capacity = 0;

                return *this;
            }

            void Allocate(u64 capacity)
            {
                ASSERT(capacity > 0);
                ASSERT(m_capacity == 0);

                if(IsInlineCapacity(capacity))
                {
                    capacity = ElementCount; // Use full available inline capacity
                    MarkUnitialized(m_union.elements, sizeof(ElementType) * capacity);

                #ifdef ENABLE_MEMORY_STATS
                    Stats::OnAllocation(sizeof(ElementType) * capacity);
                #endif
                }
                else
                {
                    new (&m_union.secondary) SecondaryAllocation();
                    m_union.secondary.Allocate(capacity);
                    ASSERT_SLOW(m_union.secondary.GetPointer() != nullptr);
                    ASSERT_SLOW(m_union.secondary.GetCapacity() > ElementCount);
                }

                m_capacity = capacity;
            }

            void Reallocate(u64 capacity)
            {
                ASSERT(capacity > 0);
                ASSERT(m_capacity != 0);

                if(m_capacity == capacity)
                    return;

                if(IsInlineCapacity(m_capacity))
                {
                    if(IsInlineCapacity(capacity))
                    {
                        if(m_capacity < capacity)
                        {
                            // Grow inline
                            MarkUnitialized(&m_union.elements[m_capacity], sizeof(ElementType) * (capacity - m_capacity));
                        }
                        else
                        {
                            // Shrink inline
                            MarkUnitialized(&m_union.elements[capacity], sizeof(ElementType) * (m_capacity - capacity));
                        }
                    }
                    else
                    {
                        // Grown inline to secondary
                        TypeStorage<ElementType> elements[ElementCount];
                        std::memcpy(elements, m_union.elements, sizeof(ElementType) * m_capacity);
                        MarkUnitialized(m_union.elements, sizeof(m_union.elements));

                    #ifdef ENABLE_MEMORY_STATS
                        Stats::OnDeallocation(sizeof(ElementType) * m_capacity);
                    #endif

                        new (&m_union.secondary) SecondaryAllocation();
                        m_union.secondary.Allocate(capacity);

                        ASSERT_SLOW(m_union.secondary.GetPointer() != nullptr);
                        ASSERT_SLOW(m_union.secondary.GetCapacity() > ElementCount);
                        std::memcpy(m_union.secondary.GetPointer(), elements, sizeof(ElementType) * m_capacity);
                    }
                }
                else
                {
                    if(IsInlineCapacity(capacity))
                    {
                        // Shrink secondary to inline
                        TypeStorage<ElementType> elements[ElementCount];
                        std::memcpy(elements, m_union.secondary.GetPointer(), sizeof(ElementType) * capacity);

                        m_union.secondary.~SecondaryAllocation();
                        MarkFreed(&m_union.secondary, sizeof(m_union.secondary));
                        ASSERT_SLOW(capacity <= ElementCount);

                        std::memcpy(m_union.elements, elements, sizeof(ElementType) * capacity);

                    #ifdef ENABLE_MEMORY_STATS
                        Stats::OnAllocation(sizeof(ElementType) * capacity);
                    #endif
                    }
                    else
                    {
                        // Reallocate secondary
                        ASSERT_SLOW(m_union.secondary.GetCapacity() > ElementCount);
                        m_union.secondary.Reallocate(capacity);
                    }
                }

                // Maintain available inline capacity at minimum
                m_capacity = Max(ElementCount, capacity);
            }

            void Deallocate()
            {
                ASSERT_SLOW(m_capacity != 0);
                if(IsInlineCapacity(m_capacity))
                {
                    MarkFreed(m_union.elements, sizeof(m_union.elements));

                #ifdef ENABLE_MEMORY_STATS
                    Stats::OnDeallocation(sizeof(ElementType) * m_capacity);
                #endif
                }
                else
                {
                    ASSERT_SLOW(m_union.secondary.GetCapacity() == m_capacity);
                    m_union.secondary.~SecondaryAllocation();
                    MarkFreed(&m_union.secondary, sizeof(m_union.secondary));
                }

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

            ElementType* GetPointer()
            {
                return const_cast<ElementType*>(std::as_const(*this).GetPointer());
            }

            const ElementType* GetPointer() const
            {
                if(m_capacity == 0)
                {
                    return nullptr;
                }

                if(IsInlineCapacity(m_capacity))
                {
                    return (ElementType*)m_union.elements;
                }

                ASSERT_SLOW(m_union.secondary.GetPointer());
                return m_union.secondary.GetPointer();
            }

            u64 GetCapacity() const
            {
                ASSERT_SLOW(m_capacity == 0 || m_capacity >= ElementCount,
                    "Inline capacity smaller than already available!");
                return m_capacity;
            }

        private:
            static bool IsInlineCapacity(u64 capacity)
            {
                return capacity <= ElementCount;
            }
        };
    };
}
