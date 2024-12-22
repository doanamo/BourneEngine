#include "Shared.hpp"
#include "Allocators/DefaultAllocator.hpp"
#include <new>

// Note: This source file should be built by executables only, not libraries.
// It will be added automatically with setup_cmake_executable() in CMakeLists.

void* operator new(const std::size_t size)
{
    return Memory::DefaultAllocator::Allocate(size, alignof(std::max_align_t));
}

void* operator new[](const std::size_t size)
{
    return Memory::DefaultAllocator::Allocate(size, alignof(std::max_align_t));
}

void* operator new(const std::size_t size, std::align_val_t alignment)
{
    return Memory::DefaultAllocator::Allocate(size, static_cast<u32>(alignment));
}

void* operator new[](const std::size_t size, std::align_val_t alignment)
{
    return Memory::DefaultAllocator::Allocate(size, static_cast<u32>(alignment));
}

void* operator new(const std::size_t size, const std::nothrow_t&) noexcept
{
    return Memory::DefaultAllocator::Allocate(size, alignof(std::max_align_t));
}

void* operator new[](const std::size_t size, const std::nothrow_t&) noexcept
{
    return Memory::DefaultAllocator::Allocate(size, alignof(std::max_align_t));
}

void* operator new(const std::size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    return Memory::DefaultAllocator::Allocate(size, static_cast<u32>(alignment));
}

void* operator new[](const std::size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    return Memory::DefaultAllocator::Allocate(size, static_cast<u32>(alignment));
}

void operator delete(void* allocation) noexcept
{
    Memory::DefaultAllocator::Deallocate(allocation, Memory::UnknownSize, alignof(std::max_align_t));
}

void operator delete[](void* allocation) noexcept
{
    Memory::DefaultAllocator::Deallocate(allocation, Memory::UnknownSize, alignof(std::max_align_t));
}

void operator delete(void* allocation, std::align_val_t alignment) noexcept
{
    Memory::DefaultAllocator::Deallocate(allocation, Memory::UnknownSize, static_cast<u32>(alignment));
}

void operator delete[](void* allocation, std::align_val_t alignment) noexcept
{
    Memory::DefaultAllocator::Deallocate(allocation, Memory::UnknownSize, static_cast<u32>(alignment));
}

void operator delete(void* allocation, const std::size_t size) noexcept
{
    Memory::DefaultAllocator::Deallocate(allocation, size, alignof(std::max_align_t));
}

void operator delete[](void* allocation, const std::size_t size) noexcept
{
    Memory::DefaultAllocator::Deallocate(allocation, size, alignof(std::max_align_t));
}

void operator delete(void* allocation, const std::size_t size, std::align_val_t alignment) noexcept
{
    Memory::DefaultAllocator::Deallocate(allocation, size, static_cast<u32>(alignment));
}

void operator delete[](void* allocation, const std::size_t size, std::align_val_t alignment) noexcept
{
    Memory::DefaultAllocator::Deallocate(allocation, size, static_cast<u32>(alignment));
}
