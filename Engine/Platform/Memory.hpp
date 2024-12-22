#pragma once

void* AlignedAlloc(u64 size, u32 alignment);
void* AlignedRealloc(void* allocation, u64 newSize, u64 oldSize, u32 alignment);
void AlignedFree(void* allocation, u64 size, u32 alignment);

#if !defined(PLATFORM_LINUX)
    const void* memmem(const void* haystack, u64 haystackSize, const void* needle, u64 needleSize);
#endif
