#include "Shared.hpp"
#include "VulkanInstance.hpp"
#include "Platform/Window.hpp"
#include "Engine.hpp"

Vulkan::Instance::Instance() = default;
Vulkan::Instance::~Instance()
{
    if(m_instance)
    {
        vkDestroyInstance(m_instance, &g_vkAllocationCallbacks);
    }
}

bool Vulkan::Instance::Setup()
{
    ASSERT(!m_instance);

    InlineArray<const char*, 2> extensions;
    extensions.Add(VK_KHR_SURFACE_EXTENSION_NAME);
    extensions.Add(Platform::Window::GetVulkanSurfaceExtension());

    VkApplicationInfo applicationInfo{};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion = VK_API_VERSION_1_3;
    applicationInfo.pEngineName = "Bourne Engine";
    applicationInfo.pApplicationName = Engine::GetApplicationName();
    applicationInfo.engineVersion = VK_MAKE_VERSION(EngineVersion::Major, EngineVersion::Minor, EngineVersion::Patch);
    applicationInfo.applicationVersion = VK_MAKE_VERSION(ApplicationVersion::Major, ApplicationVersion::Minor, ApplicationVersion::Patch);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
    createInfo.enabledExtensionCount = extensions.GetSize();
    createInfo.ppEnabledExtensionNames = extensions.GetData();

    const VkResult result = vkCreateInstance(&createInfo, &g_vkAllocationCallbacks, &m_instance);
    if(result != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create Vulkan instance (error: %s)", string_VkResult(result));
        return false;
    }

    LOG_SUCCESS("Created Vulkan instance");
    return true;
}
