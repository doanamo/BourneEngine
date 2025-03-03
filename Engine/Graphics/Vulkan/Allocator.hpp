#pragma once

namespace Vulkan
{
	void* AllocationCallback(void* userData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    void* ReallocationCallback(void* userData, void* allocation, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    void FreeCallback(void* userData, void* allocation);

    constexpr VkAllocationCallbacks g_vkAllocationCallbacks =
    {
        .pUserData = nullptr,
        .pfnAllocation = AllocationCallback,
        .pfnReallocation = ReallocationCallback,
        .pfnFree = FreeCallback,
        .pfnInternalAllocation = nullptr,
        .pfnInternalFree = nullptr
    };
}
