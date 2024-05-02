#include "Memory/Shared.hpp"
#include <new>

void* operator new(std::size_t size)
{
    return Memory::GetDefaultAllocator().Allocate(size);
}

void operator delete(void* allocation) noexcept
{
    Memory::GetDefaultAllocator().Deallocate(allocation);
}

void* operator new(std::size_t size, std::align_val_t alignment)
{
    return Memory::GetDefaultAllocator().AllocateAligned(size, static_cast<u32>(alignment));
}

void operator delete(void* allocation, std::align_val_t alignment) noexcept
{
    Memory::GetDefaultAllocator().DeallocateAligned(allocation, static_cast<u32>(alignment));
}
