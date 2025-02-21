#include "Shared.hpp"
#include "VulkanInstance.hpp"
#include "Platform/CommandLine.hpp"
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

    PrintAvailableLayers();
    InlineArray<const char*, 1> layerNames;

    const auto& commandLine = Platform::CommandLine::Get();
    if(commandLine.HasArgument("vulkanValidation"))
    {
        layerNames.Add("VK_LAYER_KHRONOS_validation");
    }

    if(!layerNames.IsEmpty())
    {
        LOG_INFO("Requested Vulkan layers:");
        for(const char* layerName : layerNames)
        {
            LOG_INFO("  %s", layerName);
        }
    }
    else
    {
        LOG_INFO("No Vulkan layers requested");
    }

    PrintAvailableExtensions();
    InlineArray<const char*, 2> extensionNames;
    extensionNames.Add(VK_KHR_SURFACE_EXTENSION_NAME);
    extensionNames.Add(Platform::Window::GetVulkanSurfaceExtension());

    LOG_INFO("Required Vulkan extensions:");
    for(const char* extensionName : extensionNames)
    {
        LOG_INFO("  %s", extensionName);
    }

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
    createInfo.enabledExtensionCount = extensionNames.GetSize();
    createInfo.ppEnabledExtensionNames = extensionNames.GetData();

    const VkResult result = vkCreateInstance(&createInfo, &g_vkAllocationCallbacks, &m_instance);
    if(result != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create Vulkan instance (error: %s)", string_VkResult(result));
        return false;
    }

    LOG_SUCCESS("Created Vulkan instance");
    return true;
}

void Vulkan::Instance::PrintAvailableLayers()
{
#ifdef ENABLE_LOGGER
    u32 layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    Array<VkLayerProperties> layers;
    layers.Resize(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layers.GetData());

    LOG_INFO("Available Vulkan layers:");
    for(const VkLayerProperties& layer : layers)
    {
        LOG_INFO("  %s", layer.layerName);
    }
#endif
}

void Vulkan::Instance::PrintAvailableExtensions()
{
#ifdef ENABLE_LOGGER
    u32 extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    Array<VkExtensionProperties> extensions;
    extensions.Resize(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.GetData());

    LOG_INFO("Available Vulkan extensions:");
    for(const VkExtensionProperties& extension : extensions)
    {
        LOG_INFO("  %s", extension.extensionName);
    }
#endif
}