#include "Memory/Shared.hpp"
#include <new>

void* operator new(std::size_t size)
{
    return Memory::GetAllocator().Allocate(size);
}

void operator delete(void* allocation) noexcept
{
    Memory::GetAllocator().Deallocate(allocation);
}

void* operator new(std::size_t size, std::align_val_t alignment)
{
    return Memory::GetAllocator().AllocateAligned(size, static_cast<u32>(alignment));
}

void operator delete(void* allocation, std::align_val_t alignment) noexcept
{
    Memory::GetAllocator().DeallocateAligned(allocation, static_cast<u32>(alignment));
}
