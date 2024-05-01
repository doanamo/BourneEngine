#include "Shared.hpp"
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

void* DefaultAllocator::Reallocate(void* allocation, u64 newSize)
{
    return realloc(allocation, newSize);
}

void DefaultAllocator::Deallocate(void* allocation)
{
    free(allocation);
}
