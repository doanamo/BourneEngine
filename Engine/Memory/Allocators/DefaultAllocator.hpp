#pragma once

namespace Memory
{
    class DefaultAllocator
    {
    public:
        static void* Allocate(u64 size, u32 alignment);
        static void* Reallocate(void* allocation, u64 requestedSize, u64 previousSize, u32 alignment);
        static void Deallocate(void* allocation, u64 size, u32 alignment);
    };
}
