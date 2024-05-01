#include "Memory/Shared.hpp"
#include "Memory/DefaultAllocator.hpp"
#include <new>

void* operator new(std::size_t size)
{
    return DefaultAllocator::Get().Allocate(size);
}

void operator delete(void* allocation) noexcept
{
    DefaultAllocator::Get().Deallocate(allocation);
}

void* operator new(std::size_t size, std::align_val_t alignment)
{
    return DefaultAllocator::Get().AllocateAligned(size, static_cast<u32>(alignment));
}

void operator delete(void* allocation, std::align_val_t alignment) noexcept
{
    DefaultAllocator::Get().DeallocateAligned(allocation, static_cast<u32>(alignment));
}
