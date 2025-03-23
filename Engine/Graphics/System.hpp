#pragma once

#if defined(GRAPHICS_VULKAN)
    #include "Vulkan/System.hpp"
#else
    #error "Unknown graphics define"
#endif

namespace Platform
{
    class Window;
}

namespace Graphics
{
    class System final : NonCopyable
    {
        Detail::System m_detail;
        Platform::Window* m_window = nullptr;

    public:
        System();
        ~System();

        bool Setup(Platform::Window* window);
        void BeginFrame();
        void EndFrame();

    private:
        bool OnSetup();
        void OnDestroy();
    };
}
