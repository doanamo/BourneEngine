#include "Shared.hpp"
#include "Allocators/DefaultAllocator.hpp"
#include <new>

void* operator new(std::size_t size)
{
    return Memory::DefaultAllocator::Allocate(size, alignof(std::max_align_t));
}

void* operator new(std::size_t size, std::align_val_t alignment)
{
    return Memory::DefaultAllocator::Allocate(size, static_cast<u32>(alignment));
}

void operator delete(void* allocation) noexcept
{
    Memory::DefaultAllocator::Deallocate(allocation, Memory::UnknownSize, alignof(std::max_align_t));
}

void operator delete(void* allocation, std::align_val_t alignment) noexcept
{
    Memory::DefaultAllocator::Deallocate(allocation, Memory::UnknownSize, static_cast<u32>(alignment));
}

// #todo: Override all known operators just in case. Optionally assert when unintended.
