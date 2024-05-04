#pragma once

#include "Allocator.hpp"

class DefaultAllocator : public Allocator
{
public:
    void* Allocate(u64 size, u32 alignment) override;
    void* Reallocate(void* allocation, u64 size, u32 alignment) override;
    void Deallocate(void* allocation, u32 alignment) override;
};
