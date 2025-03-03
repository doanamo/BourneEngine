#include "Shared.hpp"
#include "DefaultAllocator.hpp"
#include "Memory/Stats.hpp"

namespace Memory
{
#if ENABLE_MEMORY_STATS
    // Header that is placed at the beginning of each allocation.
    // This requires that every allocation is offset by aligned size while still
    // returning a pointer past the header to aligned memory usable by the user.
    struct AllocationHeader
    {
        static constexpr u8 HeaderPattern[17] = "AllocationHeader";

        u8 pattern[16] = {};
        u64 size = 0;
        u32 alignment = 0;
        bool freed = false;
        u8 padding[3] = {};

        AllocationHeader()
        {
            std::memcpy(pattern, HeaderPattern, sizeof(pattern));
        }
    };

    static_assert(sizeof(AllocationHeader) == 32);
#endif
}

void* Memory::DefaultAllocator::Allocate(const u64 size, const u32 alignment)
{
    ASSERT(size > 0);
    ASSERT(alignment != UnknownAlignment);
    ASSERT(IsPow2(alignment));

    u64 allocationSize = AlignSize(size, alignment);

#if ENABLE_MEMORY_STATS
    Stats::Get().OnAllocation(size);

    constexpr u64 headerSize = sizeof(AllocationHeader);
    const u64 headerAlignedSize = AlignSize(headerSize, alignment);
    ASSERT_SLOW(headerAlignedSize % alignment == 0, "Header size is not a multiple of alignment!");
    allocationSize += headerAlignedSize;
#endif

    u8* allocation = static_cast<u8*>(AlignedAlloc(allocationSize, alignment));
    ASSERT_ALWAYS(allocation, "Failed to allocate %llu bytes of memory with %u alignment", allocationSize, alignment);

#if ENABLE_MEMORY_STATS
    Stats::Get().OnSystemAllocation(allocationSize, headerAlignedSize);

    allocation += headerAlignedSize;
    allocationSize -= headerAlignedSize;

    auto* header = reinterpret_cast<AllocationHeader*>(allocation - headerSize);
    new (header) AllocationHeader();
    header->size = size;
    header->alignment = alignment;
    header->freed = false;
#endif

    MarkUninitialized(allocation, allocationSize);
    return allocation;
}

void* Memory::DefaultAllocator::Reallocate(void* allocation, const u64 newSize, const u64 oldSize, const u32 alignment)
{
    ASSERT(allocation);
    ASSERT(newSize > 0);
    ASSERT(alignment != UnknownAlignment);
    ASSERT(IsPow2(alignment));

    u64 newAllocationSize = AlignSize(newSize, alignment);
    u64 oldAllocationSize = oldSize != UnknownSize ? AlignSize(oldSize, alignment) : UnknownSize;

#if ENABLE_MEMORY_STATS
    constexpr u64 headerSize = sizeof(AllocationHeader);
    const u64 headerAlignedSize = AlignSize(headerSize, alignment);
    ASSERT_SLOW(headerAlignedSize % alignment == 0, "Header size is not a multiple of alignment!");

    auto* header = reinterpret_cast<AllocationHeader*>(static_cast<u8*>(allocation) - headerSize);
    ASSERT(header->size > 0, "Allocation header with invalid size!");
    ASSERT(oldSize == UnknownSize || header->size == oldSize, "Size does not match allocation header!");
    ASSERT(header->alignment == alignment, "Alignment does not match allocation header!");
    ASSERT(!header->freed, "Allocation has already been freed!");
    header->freed = true;

    Stats::Get().OnReallocation(newSize, header->size);

    if(oldSize == UnknownSize)
    {
        oldAllocationSize = AlignSize(header->size, alignment);
    }

    if(newAllocationSize < oldAllocationSize)
    {
        MarkFreed(static_cast<u8*>(allocation) + newAllocationSize, oldAllocationSize - newAllocationSize);
    }

    allocation = static_cast<u8*>(allocation) - headerAlignedSize;
    newAllocationSize += headerAlignedSize;
    oldAllocationSize += headerAlignedSize;
#endif

    u8* reallocation = static_cast<u8*>(AlignedRealloc(allocation, newAllocationSize, oldAllocationSize, alignment));
    ASSERT_ALWAYS(reallocation, "Failed to reallocate %llu bytes from %llu bytes of memory with %u alignment", newAllocationSize, oldAllocationSize, alignment);

#if ENABLE_MEMORY_STATS
    Stats::Get().OnSystemReallocation(newAllocationSize, oldAllocationSize);

    reallocation += headerAlignedSize;
    newAllocationSize -= headerAlignedSize;
    oldAllocationSize -= headerAlignedSize;

    header = reinterpret_cast<AllocationHeader*>(reallocation - headerSize);
    new (header) AllocationHeader();
    header->size = newSize;
    header->alignment = alignment;
    header->freed = false;

    if(newAllocationSize > oldAllocationSize)
    {
        MarkUninitialized(reallocation + oldAllocationSize, newAllocationSize - oldAllocationSize);
    }
#endif

    return reallocation;
}

void Memory::DefaultAllocator::Deallocate(void* allocation, const u64 size, const u32 alignment)
{
    if(allocation == nullptr)
        return;

    ASSERT(IsPow2(alignment));

    u64 allocationSize = size != UnknownSize ? AlignSize(size, alignment) : UnknownSize;

#if ENABLE_MEMORY_STATS
    constexpr u64 headerSize = sizeof(AllocationHeader);
    auto* header = reinterpret_cast<AllocationHeader*>(static_cast<u8*>(allocation) - headerSize);
    ASSERT(header->size > 0, "Allocation header with invalid size!");
    ASSERT(size == UnknownSize || header->size == size, "Size does not match allocation header!");
    ASSERT(alignment == UnknownAlignment || header->alignment == alignment, "Alignment does not match allocation header!");
    ASSERT(!header->freed, "Allocation has already been freed!");
    header->freed = true;

    if(size == UnknownSize)
    {
        allocationSize = AlignSize(header->size, header->alignment);
    }

    Stats::Get().OnDeallocation(header->size);
#endif

    MarkFreed(allocation, allocationSize);

#if ENABLE_MEMORY_STATS
    const u64 headerAlignedSize = AlignSize(headerSize, header->alignment);
    ASSERT_SLOW(headerAlignedSize % header->alignment == 0, "Header size is not a multiple of alignment!");

    allocation = static_cast<u8*>(allocation) - headerAlignedSize;
    allocationSize += headerAlignedSize;

    Stats::Get().OnSystemDeallocation(allocationSize, headerAlignedSize);
#endif

    AlignedFree(allocation, allocationSize, alignment);
}
