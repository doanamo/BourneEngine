#pragma once

namespace Memory
{
    template<typename ElementCount, typename SecondaryAllocator = Memory::DefaultAllocator>
    class InlineAllocator
    {
    public:
        static constexpr bool IsStatic = false;
        static constexpr bool NeedsType = true;

        // #todo: Implement inline allocator.
    };
}
