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

    // #todo: Rename to renderer?
    class System final : NonCopyable
    {
        Detail::System m_detail;
        Platform::Window* m_window = nullptr;
        DelegateHandle m_windowResizeDelegate;
        bool m_setup = false;

    public:
        System() = default;
        ~System();

        bool Setup(Platform::Window* window, const SystemConfig& config);
        void BeginFrame();
        void EndFrame();

    private:
        void OnResize(u32 width, u32 height);
    };
}
