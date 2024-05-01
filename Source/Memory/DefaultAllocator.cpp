#include "Shared.hpp"
#include "Memory/DefaultAllocator.hpp"
#include <cstdlib>

Memory::DefaultAllocator& Memory::DefaultAllocator::Get()
{
    static DefaultAllocator instance;
    return instance;
}

void* Memory::DefaultAllocator::Allocate(u64 size)
{
    return malloc(size);
}

void* Memory::DefaultAllocator::Reallocate(void* allocation, u64 newSize)
{
    return realloc(allocation, newSize);
}

void Memory::DefaultAllocator::Deallocate(void* allocation)
{
    free(allocation);
}
