#include "Shared.hpp"
#include "DefaultAllocator.hpp"
#include <cstdlib>

#ifdef ENABLE_MEMORY_STATS
namespace Memory
{
    std::atomic<i64> DefaultAllocator::s_allocationCount;
    std::atomic<i64> DefaultAllocator::s_allocatedTotalBytes;
    std::atomic<i64> DefaultAllocator::s_allocatedHeaderBytes;

    // Header that is placed at the beginning of each allocation.
    // This requires that every allocation is offset by aligned size of the header
    // while still returning a pointer past the header to memory usable by the user.
    struct AllocationHeader
    {
        u64 size = 0;
        u32 alignment = 0;
        bool freed = false;
    };

    static class LeakDetector
    {
    public:
        ~LeakDetector()
        {
            const i64 allocationCount = DefaultAllocator::GetAllocationCount();
            const i64 allocationBytes = DefaultAllocator::GetAllocatedTotalBytes();
            if(allocationCount != 0 || allocationBytes != 0)
            {
                LOG_ERROR("Memory leak detected: %lli allocations, %lli bytes", allocationCount, allocationBytes);
            }
        }

    } g_leakDetector;
}
#endif

void* Memory::DefaultAllocator::Allocate(u64 size, u32 alignment)
{
    ASSERT(size > 0);
    ASSERT(alignment != 0 && IsPow2(alignment));

#ifdef ENABLE_MEMORY_STATS
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    size += headerSize;
#endif

    u8* allocation = (u8*)_aligned_malloc(size, alignment);
    ASSERT_ALWAYS(allocation, "Failed to allocate %llu bytes of memory with %u alignment", size, alignment);

#ifdef ENABLE_MEMORY_STATS
    s_allocationCount.fetch_add(1, std::memory_order_relaxed);
    s_allocatedTotalBytes.fetch_add(size, std::memory_order_relaxed);
    s_allocatedHeaderBytes.fetch_add(headerSize, std::memory_order_relaxed);

    AllocationHeader* header = (AllocationHeader*)allocation;
    header->size = size - headerSize;
    header->alignment = alignment;
    header->freed = false;
    allocation += headerSize;

    FillUninitializedPattern(allocation, header->size);
#endif

    return allocation;
}

void* Memory::DefaultAllocator::Reallocate(void* allocation, u64 requestedSize, u64 currentSize, u32 alignment)
{
    ASSERT(allocation);
    ASSERT(requestedSize > 0);
    ASSERT(alignment != 0 && IsPow2(alignment));

#ifdef ENABLE_MEMORY_STATS
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    AllocationHeader* header = (AllocationHeader*)((u8*)allocation - headerSize);
    ASSERT(header->size > 0, "Allocation header with invalid size!");
    ASSERT(currentSize == 0 || header->size == currentSize, "Size does not match allocation header!");
    ASSERT(header->alignment == alignment, "Alignment does not match allocation header!");
    ASSERT(!header->freed, "Allocation has already been freed!");
    header->freed = true;

    const i64 sizeDiffference = requestedSize - header->size;
    const u64 previousSize = header->size;
    if(previousSize > requestedSize)
    {
        FillFreedPattern((u8*)allocation + requestedSize, previousSize - requestedSize);
    }

    allocation = (void*)header;
    requestedSize += headerSize;
#endif

    u8* reallocation = (u8*)_aligned_realloc(allocation, requestedSize, alignment);
    ASSERT_ALWAYS(reallocation, "Failed to reallocate %llu bytes of memory with %u alignment", requestedSize, alignment);

#ifdef ENABLE_MEMORY_STATS
    s_allocatedTotalBytes.fetch_add(sizeDiffference, std::memory_order_relaxed);

    header = (AllocationHeader*)reallocation;
    header->size = requestedSize - headerSize;
    header->alignment = alignment;
    header->freed = false;
    reallocation += headerSize;

    if(previousSize < requestedSize)
    {
        FillUninitializedPattern(reallocation + previousSize, header->size - previousSize);
    }
#endif

    return reallocation;
}

void Memory::DefaultAllocator::Deallocate(void* allocation, u64 size, u32 alignment)
{
    if(allocation == nullptr)
        return;

    ASSERT(alignment != 0 && IsPow2(alignment));

#ifdef ENABLE_MEMORY_STATS
    const u64 headerSize = AlignSize(sizeof(AllocationHeader), alignment);
    AllocationHeader* header = (AllocationHeader*)((u8*)allocation - headerSize);
    ASSERT(header->size > 0, "Allocation header with invalid size!");
    ASSERT(size == 0 || header->size == size, "Size does not match allocation header!");
    ASSERT(header->alignment == alignment, "Alignment does not match allocation header!");
    ASSERT(!header->freed, "Allocation has already been freed!");
    header->freed = true;

    s_allocationCount.fetch_sub(1, std::memory_order_relaxed);
    s_allocatedTotalBytes.fetch_sub(header->size + headerSize, std::memory_order_relaxed);
    s_allocatedHeaderBytes.fetch_sub(headerSize, std::memory_order_relaxed);

    allocation = (void*)header;
    FillFreedPattern(allocation, headerSize + header->size);
#endif

    _aligned_free(allocation);
}

#ifdef ENABLE_MEMORY_STATS
i64 Memory::DefaultAllocator::GetAllocationCount()
{
    return s_allocationCount.load(std::memory_order_relaxed);
}

i64 Memory::DefaultAllocator::GetAllocatedTotalBytes()
{
    return s_allocatedTotalBytes.load(std::memory_order_relaxed);
}

i64 Memory::DefaultAllocator::GetAllocatedHeaderBytes()
{
    return s_allocatedHeaderBytes.load(std::memory_order_relaxed);
}

i64 Memory::DefaultAllocator::GetAllocatedUsableBytes()
{
    i64 totalBytes = GetAllocatedTotalBytes();
    i64 headerBytes = GetAllocatedHeaderBytes();
    ASSERT(headerBytes <= totalBytes);
    return totalBytes - headerBytes;
}
#endif
