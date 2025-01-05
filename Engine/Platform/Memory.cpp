#include "Shared.hpp"
#include "Platform/Memory.hpp"

namespace Memory
{
    void* OnAlignedAlloc(u64 size, u32 alignment);
    void* OnAlignedRealloc(void* allocation, u64 newSize, u64 oldSize, u32 alignment);
    void OnAlignedFree(void* allocation, u64 size, u32 alignment);
}

void* Memory::AlignedAlloc(const u64 size, const u32 alignment)
{
    ASSERT(size != 0);
    ASSERT(alignment != 0);
    ASSERT(IsPow2(alignment), "Alignment is not a power of 2!");
    ASSERT(size % alignment == 0, "Allocation size is not a multiple of alignment!");

    return OnAlignedAlloc(size, alignment);
}

void* Memory::AlignedRealloc(void* allocation, const u64 newSize, const u64 oldSize, const u32 alignment)
{
    ASSERT(newSize != 0);
    ASSERT(alignment != 0);
    ASSERT(IsPow2(alignment), "Alignment is not a power of 2!");
    ASSERT(oldSize % alignment == 0, "Old allocation size is not a multiple of alignment!");
    ASSERT(newSize % alignment == 0, "New allocation size is not a multiple of alignment!");

    return OnAlignedRealloc(allocation, newSize, oldSize, alignment);
}

void Memory::AlignedFree(void* allocation, const u64 size, const u32 alignment)
{
    ASSERT(IsPow2(alignment), "Alignment is not a power of 2!");

    OnAlignedFree(allocation, size, alignment);
}
