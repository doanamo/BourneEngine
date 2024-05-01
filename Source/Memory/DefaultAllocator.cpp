#include "Memory/Shared.hpp"
#include "Memory/DefaultAllocator.hpp"
#include <cstdlib>

DefaultAllocator& DefaultAllocator::Get()
{
    static DefaultAllocator instance;
    return instance;
}

void* DefaultAllocator::Allocate(u64 size)
{
    return malloc(size);
}

void* DefaultAllocator::Reallocate(void* allocation, u64 size)
{
    return realloc(allocation, size);
}

void DefaultAllocator::Deallocate(void* allocation)
{
    free(allocation);
}

void* DefaultAllocator::AllocateAligned(u64 size, u32 alignment)
{
    return _aligned_malloc(size, alignment);
}

void* DefaultAllocator::ReallocateAligned(void* allocation, u64 size, u32 alignment)
{
    return _aligned_realloc(allocation, size, alignment);
}

void DefaultAllocator::DeallocateAligned(void* allocation)
{
    _aligned_free(allocation);
}
