#include "Shared.hpp"
#include "DefaultAllocator.hpp"
#include "Memory/MemoryStats.hpp"

namespace Memory
{
#ifdef ENABLE_MEMORY_STATS
    // Header that is placed at the beginning of each allocation.
    // This requires that every allocation is offset by aligned size while still
    // returning a pointer past the header to aligned memory usable by the user.
    struct AllocationHeader
    {
        u64 size = 0;
        u32 alignment = 0;
        bool freed = false;
    };
#endif
}

void* Memory::DefaultAllocator::Allocate(u64 size, u32 alignment)
{
    ASSERT(size > 0);
    ASSERT(alignment != 0);
    ASSERT(IsPow2(alignment));

#ifdef ENABLE_MEMORY_STATS
    const u64 requestedSize = size;
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    ASSERT_SLOW(headerSize % alignment == 0, "Header size is not a multiple of alignment!");
    size += headerSize;
#endif

    u8* allocation = (u8*)_aligned_malloc(size, alignment);
    ASSERT_ALWAYS(allocation, "Failed to allocate %llu bytes of memory with %u alignment", size, alignment);

#ifdef ENABLE_MEMORY_STATS
    Stats::OnSystemAllocation(size, headerSize);

    AllocationHeader* header = (AllocationHeader*)allocation;
    header->size = requestedSize;
    header->alignment = alignment;
    header->freed = false;
    allocation += headerSize;

    MarkUnitialized(allocation, header->size);
#endif

    return allocation;
}

void* Memory::DefaultAllocator::Reallocate(void* allocation, u64 requestedSize, u64 currentSize, u32 alignment)
{
    ASSERT(allocation);
    ASSERT(requestedSize > 0);
    ASSERT(alignment != 0);
    ASSERT(IsPow2(alignment));

#ifdef ENABLE_MEMORY_STATS
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    ASSERT_SLOW(headerSize % alignment == 0, "Header size is not a multiple of alignment!");

    AllocationHeader* header = (AllocationHeader*)((u8*)allocation - headerSize);
    ASSERT(header->size > 0, "Allocation header with invalid size!");
    ASSERT(currentSize == 0 || header->size == currentSize, "Size does not match allocation header!");
    ASSERT(header->alignment == alignment, "Alignment does not match allocation header!");
    ASSERT(!header->freed, "Allocation has already been freed!");
    header->freed = true;

    const i64 sizeDifference = requestedSize - header->size;
    const u64 previousSize = header->size;
    if(previousSize > requestedSize)
    {
        MarkFreed((u8*)allocation + requestedSize, previousSize - requestedSize);
    }

    allocation = (void*)header;
    requestedSize += headerSize;
#endif

    u8* reallocation = (u8*)_aligned_realloc(allocation, requestedSize, alignment);
    ASSERT_ALWAYS(reallocation, "Failed to reallocate %llu bytes of memory with %u alignment", requestedSize, alignment);

#ifdef ENABLE_MEMORY_STATS
    Stats::OnSystemReallocation(sizeDifference);

    header = (AllocationHeader*)reallocation;
    header->size = requestedSize - headerSize;
    header->alignment = alignment;
    header->freed = false;
    reallocation += headerSize;

    if(previousSize < requestedSize)
    {
        MarkUnitialized(reallocation + previousSize, header->size - previousSize);
    }
#endif

    return reallocation;
}

void Memory::DefaultAllocator::Deallocate(void* allocation, u64 size, u32 alignment)
{
    if(allocation == nullptr)
        return;

    ASSERT(alignment != 0);
    ASSERT(IsPow2(alignment));

#ifdef ENABLE_MEMORY_STATS
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    ASSERT_SLOW(headerSize % alignment == 0, "Header size is not a multiple of alignment!");

    AllocationHeader* header = (AllocationHeader*)((u8*)allocation - headerSize);
    ASSERT(header->size > 0, "Allocation header with invalid size!");
    ASSERT(size == 0 || header->size == size, "Size does not match allocation header!");
    ASSERT(header->alignment == alignment, "Alignment does not match allocation header!");
    ASSERT(!header->freed, "Allocation has already been freed!");
    header->freed = true;

    const u64 allocationSize = header->size + headerSize;
    Stats::OnSystemDeallocation(allocationSize, headerSize);

    allocation = (void*)header;
    MarkFreed(allocation, allocationSize);
#endif

    _aligned_free(allocation);
}
