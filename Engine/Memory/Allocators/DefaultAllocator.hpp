#pragma once

namespace Memory
{
    class DefaultAllocator
    {
    public:
        DefaultAllocator() = default;
        ~DefaultAllocator() = default;

        DefaultAllocator(const DefaultAllocator&) = default;
        DefaultAllocator& operator=(const DefaultAllocator&) = default;

        DefaultAllocator(DefaultAllocator&&) = default;
        DefaultAllocator& operator=(DefaultAllocator&&) = default;

        void* Allocate(u64 size, u32 alignment);
        void* Reallocate(void* allocation, u64 requestedSize, u64 previousSize, u32 alignment);
        void Deallocate(void* allocation, u64 size, u32 alignment);
    };
}
