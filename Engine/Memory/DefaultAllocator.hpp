#pragma once

class DefaultAllocator
{
public:
    static void* Allocate(u64 size, u32 alignment);
    static void* Reallocate(void* allocation, u64 size, u32 alignment);
    static void Deallocate(void* allocation, u32 alignment);

#ifndef CONFIG_RELEASE
    static u64 GetAllocationCount();
    static u64 GetAllocatedTotalBytes();
    static u64 GetAllocatedHeaderBytes();
    static u64 GetAllocatedUsableBytes();

private:
    static std::atomic<u64> AllocationCount;
    static std::atomic<u64> AllocatedTotalBytes;
    static std::atomic<u64> AllocatedHeaderBytes;
#endif
};
