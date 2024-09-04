#pragma once

#include "AllocatorTraits.hpp"

namespace Memory
{
    template<u64 ElementCount, typename SecondaryAllocator = Memory::DefaultAllocator>
    class InlineAllocator
    {
    public:
        // #todo: Implement inline allocator.
    };

    template<u64 ElementCount, typename SecondaryAllocator>
    struct IsAllocator<InlineAllocator<ElementCount, SecondaryAllocator>>
    {
        static const bool Value = true;
    };

    template<u64 ElementCount, typename SecondaryAllocator>
    struct IsAllocatorStatic<InlineAllocator<ElementCount, SecondaryAllocator>>
    {
        static const bool Value = false;
    };
}
