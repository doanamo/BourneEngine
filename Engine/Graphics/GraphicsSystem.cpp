#include "Shared.hpp"
#include "GraphicsSystem.hpp"
#include "GraphicsStats.hpp"
#include "Platform/Window.hpp"

Graphics::System::~System()
{
    LOG("Destroying graphics system...");

    if(m_instance)
    {
        vkDestroyInstance(m_instance, &g_vkAllocationCallbacks);
    }
}

bool Graphics::System::Setup(Platform::Window* window)
{
    LOG("Creating graphics system...");

    ASSERT(window);
    m_window = window;

    if(!CreateInstance())
    {
        LOG_ERROR("Failed to setup graphics system");
        return false;
    }

    LOG_SUCCESS("Created graphics system");
    return true;
}

bool Graphics::System::CreateInstance()
{
    InlineArray<const char*, 2> extensions;
    extensions.Add(VK_KHR_SURFACE_EXTENSION_NAME);
    extensions.Add(Platform::Window::GetVulkanExtension());

    VkApplicationInfo applicationInfo{};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion = VK_API_VERSION_1_3;
    applicationInfo.pEngineName = "Bourne Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(BuildVersion::Major, BuildVersion::Minor, BuildVersion::Patch);
    applicationInfo.pApplicationName = Engine::GetApplicationName();
    // #todo: Separate build version into engine and application versions
    //applicationInfo.applicationVersion = VK_MAKE_VERSION(BuildVersion::Major, BuildVersion::Minor, BuildVersion::Patch);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
    createInfo.enabledExtensionCount = extensions.GetSize();
    createInfo.ppEnabledExtensionNames = extensions.GetData();

    if(vkCreateInstance(&createInfo, &g_vkAllocationCallbacks, &m_instance) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create Vulkan instance");
        return false;
    }

    LOG_SUCCESS("Created Vulkan instance");
    return true;
}

void Graphics::System::BeginFrame()
{
}

void Graphics::System::EndFrame()
{
    Stats& stats = Stats::Get();
    stats.OnEndFrame();

    if(stats.HasUpdated())
    {
        auto titleStats =
            InlineString<64>::Format(" - %.2f FPS (%.2f ms)",
            stats.GetFramesPerSecond(), stats.GetFrameTimeAverage() * 1000.0f);
        m_window->SetTitleSuffix(titleStats.GetData());
    }
}
