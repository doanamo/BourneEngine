#pragma once

namespace Memory
{
    class DefaultAllocator
    {
    public:
        static void* Allocate(u64 size, u32 alignment);
        static void* Reallocate(void* allocation, u64 requestedSize, u64 previousSize, u32 alignment);
        static void Deallocate(void* allocation, u64 size, u32 alignment);

#ifndef CONFIG_RELEASE
    public:
        static i64 GetAllocationCount();
        static i64 GetAllocatedTotalBytes();
        static i64 GetAllocatedHeaderBytes();
        static i64 GetAllocatedUsableBytes();

    private:
        static std::atomic<i64> s_allocationCount;
        static std::atomic<i64> s_allocatedTotalBytes;
        static std::atomic<i64> s_allocatedHeaderBytes;
#endif
    };
}
