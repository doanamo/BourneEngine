#pragma once

namespace Vulkan
{
    class Instance
    {
        VkInstance m_instance = nullptr;

    public:
        Instance();
        ~Instance();

        bool Setup();

        static void PrintAvailableExtensions();
    };
}
