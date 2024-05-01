#pragma once

namespace Memory
{
    class Allocator
    {
    public:
        virtual void* Allocate(u64 size) = 0;
        virtual void* Reallocate(void* allocation, u64 newSize) = 0;
        virtual void Deallocate(void* allocation) = 0;
    };
}
