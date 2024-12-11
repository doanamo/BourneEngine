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

void* Memory::DefaultAllocator::Allocate(u64 size, const u32 alignment)
{
    ASSERT(size > 0);
    ASSERT(alignment != 0);
    ASSERT(IsPow2(alignment));

#ifdef ENABLE_MEMORY_STATS
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    ASSERT_SLOW(headerSize % alignment == 0, "Header size is not a multiple of alignment!");
    size += headerSize;
#endif

    u8* allocation = static_cast<u8*>(AlignedAlloc(size, alignment));
    ASSERT_ALWAYS(allocation, "Failed to allocate %llu bytes of memory with %u alignment", size, alignment);

#ifdef ENABLE_MEMORY_STATS
    Stats::OnSystemAllocation(size, headerSize);

    AllocationHeader* header = reinterpret_cast<AllocationHeader*>(allocation);
    header->size = size - headerSize;
    header->alignment = alignment;
    header->freed = false;
    allocation += headerSize;

    MarkUnitialized(allocation, header->size);
#endif

    return allocation;
}

void* Memory::DefaultAllocator::Reallocate(void* allocation, u64 requestedSize, u64 previousSize, const u32 alignment)
{
    ASSERT(allocation);
    ASSERT(requestedSize > 0);
    ASSERT(alignment != 0);
    ASSERT(IsPow2(alignment));

#ifdef ENABLE_MEMORY_STATS
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    ASSERT_SLOW(headerSize % alignment == 0, "Header size is not a multiple of alignment!");

    AllocationHeader* header = reinterpret_cast<AllocationHeader*>(static_cast<u8*>(allocation) - headerSize);
    ASSERT(header->size > 0, "Allocation header with invalid size!");
    ASSERT(previousSize == UnknownSize || header->size == previousSize, "Size does not match allocation header!");
    ASSERT(header->alignment == alignment, "Alignment does not match allocation header!");
    ASSERT(!header->freed, "Allocation has already been freed!");
    header->freed = true;

    const i64 sizeDifference = requestedSize - header->size;
    if(previousSize > requestedSize)
    {
        MarkFreed(static_cast<u8*>(allocation) + requestedSize, previousSize - requestedSize);
    }

    allocation = static_cast<void*>(header);
    requestedSize += headerSize;
    previousSize = header->size;
#endif

    u8* reallocation = static_cast<u8*>(AlignedRealloc(allocation, requestedSize, previousSize, alignment));
    ASSERT_ALWAYS(reallocation, "Failed to reallocate %llu bytes of memory with %u alignment", requestedSize, alignment);

#ifdef ENABLE_MEMORY_STATS
    Stats::OnSystemReallocation(sizeDifference);

    header = reinterpret_cast<AllocationHeader*>(reallocation);
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

void Memory::DefaultAllocator::Deallocate(void* allocation, const u64 size, const u32 alignment)
{
    if(allocation == nullptr)
        return;

    ASSERT(alignment != 0);
    ASSERT(IsPow2(alignment));

#ifdef ENABLE_MEMORY_STATS
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    ASSERT_SLOW(headerSize % alignment == 0, "Header size is not a multiple of alignment!");

    AllocationHeader* header = reinterpret_cast<AllocationHeader*>(static_cast<u8*>(allocation) - headerSize);
    ASSERT(header->size > 0, "Allocation header with invalid size!");
    ASSERT(size == UnknownSize || header->size == size, "Size does not match allocation header!");
    ASSERT(header->alignment == alignment, "Alignment does not match allocation header!");
    ASSERT(!header->freed, "Allocation has already been freed!");
    header->freed = true;

    const u64 allocationSize = header->size + headerSize;
    Stats::OnSystemDeallocation(allocationSize, headerSize);

    allocation = static_cast<void*>(header);
    MarkFreed(allocation, allocationSize);
#endif

    AlignedFree(allocation, size, alignment);
}
