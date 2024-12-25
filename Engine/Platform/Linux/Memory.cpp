#include "Shared.hpp"
#include "Platform/Memory.hpp"
#include "Includes.hpp"

namespace Memory
{
    void* OnAlignedAlloc(const u64 size, const u32 alignment)
    {
        return aligned_alloc(alignment, size);
    }

    void* OnAlignedRealloc(void* allocation, const u64 newSize, const u64 oldSize, const u32 alignment)
    {
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
            std::memcpy(reallocation, allocation, std::min(newSize, oldSize));
            free(allocation);
        }

        return reallocation;
    }

    void OnAlignedFree(void* allocation, const u64 size, const u32 alignment)
    {
        free(allocation);
    }
}
