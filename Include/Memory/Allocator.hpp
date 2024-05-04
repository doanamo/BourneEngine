#pragma once

class Allocator
{
public:
    virtual void* Allocate(u64 size, u32 alignment) = 0;
    virtual void* Reallocate(void* allocation, u64 size, u32 alignment) = 0;
    virtual void Deallocate(void* allocation, u32 alignment) = 0;
};
