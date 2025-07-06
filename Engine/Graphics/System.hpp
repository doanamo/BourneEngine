#pragma once

#if defined(GRAPHICS_D3D11)
    #include "Direct3D11/System.hpp"
#elif defined(GRAPHICS_NULL)
    #include "Null/System.hpp"
#else
    #error "Unknown graphics define"
#endif

namespace Platform
{
    class Window;
}

namespace Graphics
{
    struct SystemConfig;

    class System final : NonCopyable
    {
        Platform::Window* m_window = nullptr;
        Detail::System m_detail;
        bool m_setup = false;

    public:
        System() = default;
        ~System();

        bool Setup(Platform::Window* window, const SystemConfig& config);
        void BeginFrame();
        void EndFrame();
    };
}
