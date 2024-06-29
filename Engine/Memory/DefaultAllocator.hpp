#pragma once

#include "Allocator.hpp"

class DefaultAllocator : public Allocator
{
private:
    DefaultAllocator() = default;

public:
    static DefaultAllocator& Get();

    void* Allocate(u64 size, u32 alignment) override;
    void* Reallocate(void* allocation, u64 size, u32 alignment) override;
    void Deallocate(void* allocation) override;
};
