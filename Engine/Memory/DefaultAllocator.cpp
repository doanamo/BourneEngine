#include "Shared.hpp"
#include "DefaultAllocator.hpp"
#include <cstdlib>

#ifndef CONFIG_RELEASE

std::atomic<u64> DefaultAllocator::AllocationCount;
std::atomic<u64> DefaultAllocator::AllocatedTotalBytes;
std::atomic<u64> DefaultAllocator::AllocatedHeaderBytes;

// Header that is placed at the beginning of each allocation.
// This requires that every allocation is offset by aligned size of the header
// while still retuning a pointer past the header that remains aligned.
struct AllocationHeader
{
    // Size of allocation without header.
    u64 size = 0;

    // Alignment of allocation.
    u32 alignment = 0;
};

#endif

void* DefaultAllocator::Allocate(u64 size, u32 alignment)
{
    ASSERT(size > 0);
    ASSERT(alignment != 0);
    ASSERT(IsPow2(alignment));

#ifndef CONFIG_RELEASE
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    size += headerSize;
#endif

    u8* allocation = (u8*)_aligned_malloc(size, alignment);
    ASSERT_ALWAYS(allocation, "Failed to allocate %llu bytes of memory with %u alignment", size, alignment);

#ifndef CONFIG_RELEASE
    AllocationCount.fetch_add(1, std::memory_order_relaxed);
    AllocatedTotalBytes.fetch_add(size, std::memory_order_relaxed);
    AllocatedHeaderBytes.fetch_add(headerSize, std::memory_order_relaxed);

    AllocationHeader* header = (AllocationHeader*)allocation;
    header->size = size - headerSize;
    header->alignment = alignment;
    allocation += headerSize;
#endif

    return allocation;
}

void* DefaultAllocator::Reallocate(void* allocation, u64 size, u32 alignment)
{
    ASSERT(allocation);
    ASSERT(size > 0);
    ASSERT(IsPow2(alignment));

#ifndef CONFIG_RELEASE
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    AllocationHeader* header = (AllocationHeader*)((u8*)allocation - headerSize);
    ASSERT(header->size > 0);
    ASSERT(header->alignment == alignment);

    const u64 sizeDifference = size - header->size;
    allocation = (void*)header;
    size += headerSize;
#endif

    u8* reallocation = (u8*)_aligned_realloc(allocation, size, alignment);
    ASSERT_ALWAYS(reallocation, "Failed to reallocate %llu bytes of memory with %u alignment", size, alignment);

#ifndef CONFIG_RELEASE
    AllocatedTotalBytes.fetch_add(sizeDifference, std::memory_order_relaxed);

    header = (AllocationHeader*)reallocation;
    header->size = size - headerSize;
    header->alignment = alignment;
    reallocation += headerSize;
#endif

    return reallocation;
}

void DefaultAllocator::Deallocate(void* allocation, u32 alignment)
{
    ASSERT(IsPow2(alignment));

#ifndef CONFIG_RELEASE
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    AllocationHeader* header = (AllocationHeader*)((u8*)allocation - headerSize);
    ASSERT(header->size > 0);
    ASSERT(header->alignment == alignment);

    AllocationCount.fetch_sub(1, std::memory_order_relaxed);
    AllocatedTotalBytes.fetch_sub(header->size + headerSize, std::memory_order_relaxed);
    AllocatedHeaderBytes.fetch_sub(headerSize, std::memory_order_relaxed);

    allocation = (void*)header;
#endif

    _aligned_free(allocation);
}

#ifndef CONFIG_RELEASE

u64 DefaultAllocator::GetAllocationCount()
{
    return AllocationCount.load(std::memory_order_relaxed);
}

u64 DefaultAllocator::GetAllocatedTotalBytes()
{
    return AllocatedTotalBytes.load(std::memory_order_relaxed);
}

u64 DefaultAllocator::GetAllocatedHeaderBytes()
{
    return AllocatedHeaderBytes.load(std::memory_order_relaxed);
}

u64 DefaultAllocator::GetAllocatedUsableBytes()
{
    u64 totalBytes = GetAllocatedTotalBytes();
    u64 headerBytes = GetAllocatedHeaderBytes();
    ASSERT(headerBytes <= totalBytes);
    return totalBytes - headerBytes;
}

#endif
