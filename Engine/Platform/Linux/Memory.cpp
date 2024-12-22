#include "Shared.hpp"
#include "Platform/Memory.hpp"

void* AlignedAlloc(const u64 size, const u32 alignment)
{
    ASSERT_SLOW(size != 0 && alignment != 0);
    ASSERT_SLOW(IsPow2(alignment), "Alignment is not a power of 2!");
    ASSERT_SLOW(size % alignment == 0, "Allocation size is not a multiple of alignment!");

    return aligned_alloc(alignment, size);
}

void* AlignedRealloc(void* allocation, const u64 newSize, const u64 oldSize, const u32 alignment)
{
    ASSERT_SLOW(newSize != 0 &&oldSize != 0 && alignment != 0);
    ASSERT_SLOW(IsPow2(alignment), "Alignment is not a power of 2!");
    ASSERT_SLOW(oldSize % alignment == 0, "Old allocation size is not a multiple of alignment!");
    ASSERT_SLOW(newSize % alignment == 0, "New allocation size is not a multiple of alignment!");

    if(allocation == nullptr)
    {
        return aligned_alloc(alignment, newSize);
    }

    // Note: This implementation purposely does not reuse allocation when shrinking, because it would waste space.
    // Other system implementations (like _aligned_realloc()) would free some pages/blocks while keeping same pointer.
    // If we would like to optimize this, we are better off writing our own low-level allocator based on system pages.
    void* reallocation = aligned_alloc(alignment, newSize);
    if (reallocation)
    {
        std::memcpy(reallocation, allocation, Min(newSize, oldSize));
        free(allocation);
    }

    return reallocation;
}

void AlignedFree(void* allocation, const u64 size, const u32 alignment)
{
    ASSERT_SLOW(size != 0 && alignment != 0);
    ASSERT_SLOW(IsPow2(alignment), "Alignment is not a power of 2!");
    ASSERT_SLOW(size % alignment == 0, "Allocation size is not a multiple of alignment!");

    free(allocation);
}

const void* Memmem(const void* haystack, const u64 haystackSize, const void* needle, const u64 needleSize)
{
    return memmem(haystack, haystackSize, needle, needleSize);
}
