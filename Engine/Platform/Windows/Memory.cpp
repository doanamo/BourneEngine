#include "Shared.hpp"
#include "Platform/Memory.hpp"

namespace Memory
{
    void* OnAlignedAlloc(const u64 size, const u32 alignment)
    {
        return _aligned_malloc(size, alignment);
    }

    void* OnAlignedRealloc(void* allocation, const u64 newSize, const u64 oldSize, const u32 alignment)
    {
        return _aligned_realloc(allocation, newSize, alignment);
    }

    void OnAlignedFree(void* allocation, const u64 size, const u32 alignment)
    {
        _aligned_free(allocation);
    }
}

const void* memmem(const void* haystack, const u64 haystackSize, const void* needle, const u64 needleSize)
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
