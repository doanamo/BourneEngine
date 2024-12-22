#include "Shared.hpp"
#include "Platform/Memory.hpp"

void* OnAlignedAlloc(u64 size, u32 alignment);
void* OnAlignedRealloc(void* allocation, u64 newSize, u64 oldSize, u32 alignment);
void OnAlignedFree(void* allocation, u64 size, u32 alignment);

void* AlignedAlloc(const u64 size, const u32 alignment)
{
    ASSERT_SLOW(size != 0 && alignment != 0);
    ASSERT_SLOW(IsPow2(alignment), "Alignment is not a power of 2!");
    ASSERT_SLOW(size % alignment == 0, "Allocation size is not a multiple of alignment!");

    return OnAlignedAlloc(size, alignment);
}

void* AlignedRealloc(void* allocation, const u64 newSize, const u64 oldSize, const u32 alignment)
{
    ASSERT_SLOW(newSize != 0 &&oldSize != 0 && alignment != 0);
    ASSERT_SLOW(IsPow2(alignment), "Alignment is not a power of 2!");
    ASSERT_SLOW(oldSize % alignment == 0, "Old allocation size is not a multiple of alignment!");
    ASSERT_SLOW(newSize % alignment == 0, "New allocation size is not a multiple of alignment!");

    return OnAlignedRealloc(allocation, newSize, oldSize, alignment);
}

void AlignedFree(void* allocation, const u64 size, const u32 alignment)
{
    ASSERT_SLOW(size != 0 && alignment != 0);
    ASSERT_SLOW(IsPow2(alignment), "Alignment is not a power of 2!");
    ASSERT_SLOW(size % alignment == 0, "Allocation size is not a multiple of alignment!");

    OnAlignedFree(allocation, size, alignment);
}
