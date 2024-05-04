#include "Memory/Shared.hpp"
#include "Memory/DefaultAllocator.hpp"
#include <cstdlib>

void* DefaultAllocator::Allocate(u64 size, u32 alignment)
{
    void* allocation = _aligned_malloc(size, alignment);
    ASSERT_ALWAYS(allocation, "Failed to allocate %llu bytes of memory with %u alignment");
    return allocation;
}

void* DefaultAllocator::Reallocate(void* allocation, u64 size, u32 alignment)
{
    return _aligned_realloc(allocation, size, alignment);
}

void DefaultAllocator::Deallocate(void* allocation, u32 alignment)
{
    _aligned_free(allocation);
}
