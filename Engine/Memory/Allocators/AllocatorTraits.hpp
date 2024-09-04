#pragma once

namespace Memory
{
    // Indicates whether the type is an allocator.
    template<typename AllocatorType>
    struct IsAllocator
    {
        static const bool Value = false;
    };

    // Indicates whether allocator can be called statically without an instance.
    // This requires that the allocator interface is stateless.
    // Must implement Allocate/Reallocate/Deallocate functions.
    template<typename AllocatorType>
    struct IsAllocatorStatic
    {
        static const bool Value = false;
    };
}
