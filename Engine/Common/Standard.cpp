#include "Shared.hpp"
#include "Standard.hpp"

void* AlignedAlloc(const u64 size, const u32 alignment)
{
    ASSERT_SLOW(size != 0 && alignment != 0);
    ASSERT_SLOW(IsPow2(alignment), "Alignment is not a power of 2!");
    ASSERT_SLOW(size % alignment == 0, "Allocation size is not a multiple of alignment!");

#if defined(PLATFORM_WINDOWS)
    return _aligned_malloc(size, alignment);
#elif defined(PLATFORM_LINUX)
    return aligned_alloc(alignment, size);
#else
    #error Unknown platform!
#endif
}

void* AlignedRealloc(void* allocation, const u64 newSize, const u64 oldSize, const u32 alignment)
{
    ASSERT_SLOW(newSize != 0 &&oldSize != 0 && alignment != 0);
    ASSERT_SLOW(IsPow2(alignment), "Alignment is not a power of 2!");
    ASSERT_SLOW(oldSize % alignment == 0, "Old allocation size is not a multiple of alignment!");
    ASSERT_SLOW(newSize % alignment == 0, "New allocation size is not a multiple of alignment!");

#if defined(PLATFORM_WINDOWS)
    return _aligned_realloc(allocation, newSize, alignment);
#elif defined(PLATFORM_LINUX)
    if(allocation == nullptr)
        return aligned_alloc(alignment, newSize);

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
#else
    #error Unknown platform!
#endif
}

void AlignedFree(void* allocation, const u64 size, const u32 alignment)
{
    ASSERT_SLOW(size != 0 && alignment != 0);
    ASSERT_SLOW(IsPow2(alignment), "Alignment is not a power of 2!");
    ASSERT_SLOW(size % alignment == 0, "Allocation size is not a multiple of alignment!");

#if defined(PLATFORM_WINDOWS)
    _aligned_free(allocation);
#elif defined(PLATFORM_LINUX)
    free(allocation);
#else
    #error Unknown platform!
#endif
}

const void* Memmem(const void* haystack, const u64 haystackSize, const void* needle, const u64 needleSize)
{
    if(haystack == nullptr || haystackSize == 0)
        return nullptr;

    if(needle == nullptr || needleSize == 0)
        return nullptr;

    const u64* iterator = static_cast<const u64*>(haystack);
    while(true)
    {
        const u64 sizeRemaining = haystackSize - (iterator - static_cast<const u64*>(haystack));
        if(sizeRemaining < needleSize)
            break;

        iterator = static_cast<const u64*>(std::memchr(iterator,
            *static_cast<const u8*>(needle), sizeRemaining - needleSize + 1));

        if(iterator == nullptr)
            break;

        if(std::memcmp(iterator, needle, needleSize) == 0)
            return iterator;

        iterator++;
    }

    return nullptr;
}
