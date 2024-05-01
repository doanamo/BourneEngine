#pragma once

#include "Allocator.hpp"

class DefaultAllocator : public Allocator
{
public:
    static DefaultAllocator& Get();

    void* Allocate(u64 size) override;
    void* Reallocate(void* allocation, u64 newSize) override;
    void Deallocate(void* allocation) override;
};
