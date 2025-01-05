#pragma once

void* VulkanAllocationCallback(void* userData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
void* VulkanReallocationCallback(void* userData, void* allocation, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
void VulkanFreeCallback(void* userData, void* allocation);

constexpr VkAllocationCallbacks g_vkAllocationCallbacks =
{
    .pUserData = nullptr,
    .pfnAllocation = VulkanAllocationCallback,
    .pfnReallocation = VulkanReallocationCallback,
    .pfnFree = VulkanFreeCallback,
    .pfnInternalAllocation = nullptr,
    .pfnInternalFree = nullptr
};
