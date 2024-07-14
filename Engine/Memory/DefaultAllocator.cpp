#include "Shared.hpp"
#include "DefaultAllocator.hpp"
#include <cstdlib>

#ifndef CONFIG_RELEASE

std::atomic<u64> Memory::DefaultAllocator::s_allocationCount;
std::atomic<u64> Memory::DefaultAllocator::s_allocatedTotalBytes;
std::atomic<u64> Memory::DefaultAllocator::s_allocatedHeaderBytes;

// Header that is placed at the beginning of each allocation.
// This requires that every allocation is offset by aligned size of the header
// while still returning a pointer past the header to memory usable by the user.
namespace Memory
{
    struct AllocationHeader
    {
        // Size of allocation without header.
        u64 size = 0;

        // Alignment of allocation.
        u32 alignment = 0;
    };
}

#endif

void* Memory::DefaultAllocator::Allocate(u64 size, u32 alignment)
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
    s_allocationCount.fetch_add(1, std::memory_order_relaxed);
    s_allocatedTotalBytes.fetch_add(size, std::memory_order_relaxed);
    s_allocatedHeaderBytes.fetch_add(headerSize, std::memory_order_relaxed);

    AllocationHeader* header = (AllocationHeader*)allocation;
    header->size = size - headerSize;
    header->alignment = alignment;
    allocation += headerSize;
#endif

    return allocation;
}

void* Memory::DefaultAllocator::Reallocate(void* allocation, u64 size, u32 alignment)
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
    s_allocatedTotalBytes.fetch_add(sizeDifference, std::memory_order_relaxed);

    header = (AllocationHeader*)reallocation;
    header->size = size - headerSize;
    header->alignment = alignment;
    reallocation += headerSize;
#endif

    return reallocation;
}

void Memory::DefaultAllocator::Deallocate(void* allocation, u32 alignment)
{
    ASSERT(IsPow2(alignment));

#ifndef CONFIG_RELEASE
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    AllocationHeader* header = (AllocationHeader*)((u8*)allocation - headerSize);
    ASSERT(header->size > 0);
    ASSERT(header->alignment == alignment);

    s_allocationCount.fetch_sub(1, std::memory_order_relaxed);
    s_allocatedTotalBytes.fetch_sub(header->size + headerSize, std::memory_order_relaxed);
    s_allocatedHeaderBytes.fetch_sub(headerSize, std::memory_order_relaxed);

    allocation = (void*)header;
#endif

    _aligned_free(allocation);
}

#ifndef CONFIG_RELEASE

u64 Memory::DefaultAllocator::GetAllocationCount()
{
    return s_allocationCount.load(std::memory_order_relaxed);
}

u64 Memory::DefaultAllocator::GetAllocatedTotalBytes()
{
    return s_allocatedTotalBytes.load(std::memory_order_relaxed);
}

u64 Memory::DefaultAllocator::GetAllocatedHeaderBytes()
{
    return s_allocatedHeaderBytes.load(std::memory_order_relaxed);
}

u64 Memory::DefaultAllocator::GetAllocatedUsableBytes()
{
    u64 totalBytes = GetAllocatedTotalBytes();
    u64 headerBytes = GetAllocatedHeaderBytes();
    ASSERT(headerBytes <= totalBytes);
    return totalBytes - headerBytes;
}

#endif
