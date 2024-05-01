#pragma once

#include "Allocator.hpp"

class DefaultAllocator : public Allocator
{
public:
    static DefaultAllocator& Get();

    void* Allocate(u64 size) override;
    void* Reallocate(void* allocation, u64 size) override;
    void Deallocate(void* allocation) override;

    void* AllocateAligned(u64 size, u32 alignment) override;
    void* ReallocateAligned(void* allocation, u64 size, u32 alignment) override;
    void DeallocateAligned(void* allocation) override;

};
