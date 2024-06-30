#pragma once

class DefaultAllocator
{
public:
    static void* Allocate(u64 size, u32 alignment);
    static void* Reallocate(void* allocation, u64 size, u32 alignment);
    static void Deallocate(void* allocation, u32 alignment);
};
