#include "Precompiled.hpp"
#include "DefaultAllocator.hpp"
#include <cstdlib>

void* DefaultAllocator::Allocate(u64 size, u32 alignment)
{
    ASSERT(size >= 0);
    ASSERT(alignment != 0);
    ASSERT(IsPow2(alignment));
    
    void* allocation = _aligned_malloc(size, alignment);
    ASSERT_ALWAYS(allocation, "Failed to allocate %llu bytes of memory with %u alignment", size, alignment);
    return allocation;
}

void* DefaultAllocator::Reallocate(void* allocation, u64 size, u32 alignment)
{
    ASSERT(allocation);
    ASSERT(size >= 0);
    ASSERT(IsPow2(alignment));

    void* reallocation = _aligned_realloc(allocation, size, alignment);
    ASSERT_ALWAYS(reallocation, "Failed to reallocate %llu bytes of memory with %u alignment", size, alignment);
    return reallocation;
}

void DefaultAllocator::Deallocate(void* allocation, u32 alignment)
{
    ASSERT(IsPow2(alignment));
    _aligned_free(allocation);
}
