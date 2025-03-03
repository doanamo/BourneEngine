#include "Shared.hpp"
#include "Allocator.hpp"

void* Vulkan::AllocationCallback(void* userData, const size_t size, const size_t alignment, VkSystemAllocationScope allocationScope)
{
    return Memory::Allocators::Default::Allocate(size, alignment);
}

void* Vulkan::ReallocationCallback(void* userData, void* allocation, const size_t size, const size_t alignment, VkSystemAllocationScope allocationScope)
{
    return Memory::Allocators::Default::Reallocate(allocation, size, Memory::UnknownSize, alignment);
}

void Vulkan::FreeCallback(void* userData, void* allocation)
{
    Memory::Allocators::Default::Deallocate(allocation, Memory::UnknownSize, Memory::UnknownAlignment);
}
