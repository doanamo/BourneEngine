#include "Shared.hpp"
#include "VulkanAllocator.hpp"

void* VulkanAllocationCallback(void* userData, const size_t size, const size_t alignment, VkSystemAllocationScope allocationScope)
{
    return Memory::DefaultAllocator::Allocate(size, alignment);
}

void* VulkanReallocationCallback(void* userData, void* allocation, const size_t size, const size_t alignment, VkSystemAllocationScope allocationScope)
{
    return Memory::DefaultAllocator::Reallocate(allocation, size, Memory::UnknownSize, alignment);
}

void VulkanFreeCallback(void* userData, void* allocation)
{
    Memory::DefaultAllocator::Deallocate(allocation, Memory::UnknownSize, Memory::UnknownAlignment);
}
