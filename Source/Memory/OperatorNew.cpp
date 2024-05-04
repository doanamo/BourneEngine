#include "Memory/Shared.hpp"
#include <new>

void* operator new(std::size_t size)
{
    return Memory::GetDefaultAllocator().Allocate(size, alignof(std::max_align_t));
}

void operator delete(void* allocation) noexcept
{
    Memory::GetDefaultAllocator().Deallocate(allocation, alignof(std::max_align_t));
}

void* operator new(std::size_t size, std::align_val_t alignment)
{
    return Memory::GetDefaultAllocator().Allocate(size, static_cast<u32>(alignment));
}

void operator delete(void* allocation, std::align_val_t alignment) noexcept
{
    Memory::GetDefaultAllocator().Deallocate(allocation, static_cast<u32>(alignment));
}
