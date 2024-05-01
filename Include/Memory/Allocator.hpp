#pragma once

class Allocator
{
public:
    virtual void* Allocate(u64 size) = 0;
    virtual void* Reallocate(void* allocation, u64 size) = 0;
    virtual void Deallocate(void* allocation) = 0;

    virtual void* AllocateAligned(u64 size, u32 alignment) = 0;
    virtual void* ReallocateAligned(void* allocation, u64 size, u32 alignment) = 0;
    virtual void DeallocateAligned(void* allocation, u32 alignment) = 0;
};
