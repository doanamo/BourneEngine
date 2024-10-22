#pragma once

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

            static bool IsInlineCapacity(u64 capacity)
            {
                return capacity <= ElementCount;
            }

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
                if(IsInlineCapacity(m_capacity))
                {
                    if(IsInlineCapacity(other.m_capacity))
                    {
                        if(other.m_capacity == 0)
                        {
                            if(m_capacity == 0)
                            {
                                // Empty to empty
                                return *this;
                            }

                            // Inline to empty
                            other.Allocate(m_capacity);
                        }

                        // Inline to inline
                        TypeStorage<ElementType> elements[ElementCount];
                        memcpy(elements, m_union.elements, sizeof(ElementType) * m_capacity);
                        memcpy(m_union.elements, other.m_union.elements, sizeof(ElementType) * other.m_capacity);
                        memcpy(other.m_union.elements, elements, sizeof(ElementType) * m_capacity);

                        MarkUnitialized(&m_union.elements[other.m_capacity], sizeof(ElementType) * (ElementCount - other.m_capacity));
                        MarkUnitialized(&other.m_union.elements[m_capacity], sizeof(ElementType) * (ElementCount - m_capacity));
                    }
                    else if(m_capacity == 0)
                    {
                        // Empty to secondary
                        ASSERT_SLOW(other.m_capacity > ElementCount);
                        new (&m_union.secondary) SecondaryAllocation();
                        std::swap(m_union.secondary, other.m_union.secondary);
                    }
                    else
                    {
                        // Inline to secondary
                        TypeStorage<ElementType> elements[ElementCount];
                        memcpy(elements, m_union.elements, sizeof(ElementType) * m_capacity);
                        MarkUnitialized(m_union.elements, sizeof(m_union.elements));

                        ASSERT_SLOW(other.m_capacity > ElementCount);
                        new (&m_union.secondary) SecondaryAllocation();
                        std::swap(m_union.secondary, other.m_union.secondary);

                        m_union.secondary.~SecondaryAllocation();
                        MarkUnitialized(&other.m_union.secondary, sizeof(other.m_union.secondary));

                        ASSERT_SLOW(m_union.secondary.GetPointer() != nullptr);
                        ASSERT_SLOW(m_union.secondary.GetCapacity() > ElementCount);
                        memcpy(other.m_union.elements, elements, sizeof(ElementType) * m_capacity);
                    }
                }
                else
                {
                    if(IsInlineCapacity(other.m_capacity)) // Secondary to inline
                    {
                        TypeStorage<ElementType> otherElements[ElementCount];
                        memcpy(otherElements, other.m_union.elements, sizeof(ElementType) * other.m_capacity);
                        MarkUnitialized(other.m_union.elements, sizeof(other.m_union.elements));

                        new (&other.m_union.secondary) SecondaryAllocation();
                        std::swap(other.m_union.secondary, m_union.secondary);

                        ASSERT_SLOW(m_capacity > ElementCount);
                        m_union.secondary.~SecondaryAllocation();
                        MarkUnitialized(&m_union.secondary, sizeof(m_union.secondary));

                        ASSERT_SLOW(other.m_union.secondary.GetPointer() != nullptr);
                        ASSERT_SLOW(other.m_union.secondary.GetCapacity() > ElementCount);
                        memcpy(m_union.elements, otherElements, sizeof(ElementType) * other.m_capacity);
                    }
                    if(other.m_capacity == 0) // Secondary to empty
                    {
                        ASSERT_SLOW(m_capacity > ElementCount);
                        new (&other.m_union.secondary) SecondaryAllocation();
                        std::swap(other.m_union.secondary, m_union.secondary);

                        m_union.secondary.~SecondaryAllocation();
                        MarkUnitialized(&m_union.secondary, sizeof(m_union.secondary));
                    }
                    else // Secondary to secondary
                    {
                        ASSERT_SLOW(m_capacity > ElementCount);
                        ASSERT_SLOW(other.m_capacity > ElementCount);
                        std::swap(m_union.secondary, other.m_union.secondary);
                    }
                }

                std::swap(m_capacity, other.m_capacity);
                return *this;
            }

            void Allocate(u64 capacity)
            {
                ASSERT(capacity > 0);
                ASSERT(m_capacity == 0);

                if(IsInlineCapacity(capacity))
                {
                    capacity = ElementCount; // Use full inline capacity
                    MarkUnitialized(m_union.elements, sizeof(ElementType) * capacity);
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
                        memcpy(elements, m_union.elements, sizeof(ElementType) * m_capacity);
                        MarkUnitialized(m_union.elements, sizeof(m_union.elements));

                        new (&m_union.secondary) SecondaryAllocation();
                        m_union.secondary.Allocate(capacity);

                        ASSERT_SLOW(m_union.secondary.GetPointer() != nullptr);
                        ASSERT_SLOW(m_union.secondary.GetCapacity() > ElementCount);
                        memcpy(m_union.secondary.GetPointer(), elements, sizeof(ElementType) * m_capacity);
                    }
                }
                else
                {
                    if(IsInlineCapacity(capacity))
                    {
                        // Shrink secondary to inline
                        TypeStorage<ElementType> elements[ElementCount];
                        memcpy(elements, m_union.secondary.GetPointer(), sizeof(ElementType) * capacity);

                        m_union.secondary.Deallocate();
                        m_union.secondary.~SecondaryAllocation();
                        ASSERT_SLOW(capacity <= ElementCount);

                        MarkUnitialized(m_union.elements, sizeof(m_union.elements));
                        memcpy(m_union.elements, elements, sizeof(ElementType) * capacity);
                    }
                    else
                    {
                        // Reallocate secondary
                        ASSERT_SLOW(m_union.secondary.GetCapacity() > ElementCount);
                        m_union.secondary.Reallocate(capacity);
                    }
                }

                m_capacity = capacity;
            }

            void Deallocate()
            {
                ASSERT_SLOW(m_capacity != 0);
                if(IsInlineCapacity(m_capacity))
                {
                    MarkFreed(m_union.elements, sizeof(m_union.elements));
                }
                else
                {
                    ASSERT_SLOW(m_union.secondary.GetCapacity() == m_capacity);
                    m_union.secondary.Deallocate();
                    m_union.secondary.~SecondaryAllocation();
                }

                m_capacity = 0;
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
                return m_capacity;
            }
        };
    };
}
