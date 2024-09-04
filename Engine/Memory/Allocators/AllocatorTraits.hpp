#pragma once

namespace Memory
{
    template<typename AllocatorType>
    struct IsAllocator
    {
        static const bool Value = false;
    };

    template<typename AllocatorType>
    struct IsAllocatorStatic
    {
        static const bool Value = false;
    };
}
