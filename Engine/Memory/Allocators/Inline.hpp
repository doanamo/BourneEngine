#pragma once

#include "Memory/Memory.hpp"
#include "Memory/Stats.hpp"

namespace Memory::Allocators
{
    template<u64 ElementCount, typename SecondaryAllocator = Default>
    class Inline final
    {
    public:
        Inline() = delete;

        template<typename ElementType>
        class TypedAllocation final
        {
            struct PrimaryAllocation
            {
                ObjectStorage<ElementType> elements[ElementCount];

                PrimaryAllocation()
                {
                    MarkUninitialized(elements, sizeof(ElementType) * ElementCount);

                #if ENABLE_MEMORY_STATS
                    Stats::Get().OnInlineAllocation(sizeof(ElementType) * ElementCount);
                #endif
                }

                PrimaryAllocation(const PrimaryAllocation& other) = delete;
                PrimaryAllocation& operator=(const PrimaryAllocation& other) = delete;

                PrimaryAllocation(PrimaryAllocation&& other) noexcept
                {
                    *this = Move(other);

                #if ENABLE_MEMORY_STATS
                    Stats::Get().OnInlineAllocation(sizeof(ElementType) * ElementCount);
                #endif
                }

                PrimaryAllocation& operator=(PrimaryAllocation&& other) noexcept
                {
                    std::memcpy(elements, other.elements, sizeof(ElementType) * ElementCount);
                    MarkUninitialized(other.elements, sizeof(ElementType) * ElementCount);
                    return *this;
                }

                ~PrimaryAllocation()
                {
                    MarkFreed(elements, sizeof(ElementType) * ElementCount);

                #if ENABLE_MEMORY_STATS
                    Stats::Get().OnInlineDeallocation(sizeof(ElementType) * ElementCount);
                #endif
                }
            };

            using SecondaryAllocation = typename SecondaryAllocator::template TypedAllocation<ElementType>;
            using StorageType = std::variant<PrimaryAllocation, SecondaryAllocation>;

            StorageType m_storage;

        public:
            TypedAllocation() = default;
            ~TypedAllocation() = default;

            TypedAllocation(const TypedAllocation&) = delete;
            TypedAllocation& operator=(const TypedAllocation&) = delete;

            TypedAllocation(TypedAllocation&& other) noexcept
            {
                *this = Move(other);
            }

            TypedAllocation& operator=(TypedAllocation&& other) noexcept
            {
                m_storage = Move(other.m_storage);
                other.m_storage.template emplace<PrimaryAllocation>();
                return *this;
            }

            void Allocate(const u64 capacity)
            {
                ASSERT(false, "Inline allocator is always in allocated state");
            }

            void Reallocate(const u64 newCapacity, const u64 usedCapacity)
            {
                const u64 oldCapacity = GetCapacity();

                ASSERT(newCapacity > 0);
                ASSERT(oldCapacity != 0);
                ASSERT(usedCapacity <= oldCapacity);

                if(newCapacity == oldCapacity)
                    return;

                if(IsInlineCapacity(oldCapacity))
                {
                    auto& primary = std::get<PrimaryAllocation>(m_storage);
                    if(IsInlineCapacity(newCapacity))
                    {
                        // Inline keeps its max capacity
                    }
                    else
                    {
                        // Grown inline to secondary
                        ObjectStorage<ElementType> elements[ElementCount];
                        std::memcpy(elements, primary.elements, sizeof(ElementType) * usedCapacity);

                        auto& secondary = m_storage.template emplace<SecondaryAllocation>();
                        secondary.Allocate(newCapacity);

                        ASSERT_SLOW(secondary.GetCapacity() >= ElementCount);
                        std::memcpy(secondary.GetPointer(), elements, sizeof(ElementType) * usedCapacity);
                    }
                }
                else
                {
                    auto& secondary = std::get<SecondaryAllocation>(m_storage);
                    if(IsInlineCapacity(newCapacity))
                    {
                        // Shrink secondary to inline
                        ObjectStorage<ElementType> elements[ElementCount];
                        std::memcpy(elements, secondary.GetPointer(), sizeof(ElementType) * newCapacity);

                        auto& primary = m_storage.template emplace<PrimaryAllocation>();
                        std::memcpy(primary.elements, elements, sizeof(ElementType) * newCapacity);
                    }
                    else
                    {
                        secondary.Reallocate(newCapacity, usedCapacity);
                    }
                }
            }

            void Deallocate()
            {
                if(auto* secondary = std::get_if<SecondaryAllocation>(&m_storage))
                {
                    secondary->Deallocate();
                    m_storage.template emplace<PrimaryAllocation>();
                }
            }

            void Resize(const u64 newCapacity, const u64 usedCapacity)
            {
                if(GetCapacity() != 0)
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

            ElementType* GetPointer()
            {
                return const_cast<ElementType*>(std::as_const(*this).GetPointer());
            }

            const ElementType* GetPointer() const
            {
                if(const auto* primary = std::get_if<PrimaryAllocation>(&m_storage))
                {
                    return reinterpret_cast<const ElementType*>(primary->elements);
                }

                if(const auto* secondary = std::get_if<SecondaryAllocation>(&m_storage))
                {
                    return secondary->GetPointer();
                }

                return nullptr;
            }

            u64 GetCapacity() const
            {
                if(const auto* primary = std::get_if<PrimaryAllocation>(&m_storage))
                {
                    return ElementCount;
                }

                if(const auto* secondary = std::get_if<SecondaryAllocation>(&m_storage))
                {
                    return secondary->GetCapacity();
                }

                return 0;
            }

        private:
            static bool IsInlineCapacity(const u64 capacity)
            {
                return capacity <= ElementCount;
            }
        };
    };
}
