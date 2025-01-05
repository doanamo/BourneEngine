#pragma once

#include "GraphicsStats.hpp"

namespace Platform
{
    class Window;
}

namespace Graphics
{
    class System final
    {
        Platform::Window* m_window = nullptr;
        VkInstance m_instance;

    public:
        System() = default;
        ~System();

        System(const System&) = delete;
        System& operator=(const System&) = delete;

        bool Setup(Platform::Window* window);
        void BeginFrame();
        void EndFrame();

    private:
        bool CreateInstance();
        void DestroyInstance();
    };
}
