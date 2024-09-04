#pragma once

#include "AllocatorTraits.hpp"

namespace Memory
{
    class DefaultAllocator
    {
    public:
        static void* Allocate(u64 size, u32 alignment);
        static void* Reallocate(void* allocation, u64 requestedSize, u64 previousSize, u32 alignment);
        static void Deallocate(void* allocation, u64 size, u32 alignment);
    };

    template<>
    struct IsAllocator<DefaultAllocator>
    {
        static const bool Value = true;
    };

    template<>
    struct IsAllocatorStatic<DefaultAllocator>
    {
        static const bool Value = true;
    };
}
