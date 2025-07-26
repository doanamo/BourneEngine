#pragma once

#if defined(GRAPHICS_D3D11)
    #include "Direct3D11/RenderApi.hpp"
#elif defined(GRAPHICS_NULL)
    #include "Null/RenderApi.hpp"
#else
    #error "Unknown graphics define"
#endif

namespace Platform
{
    class Window;
}

namespace Graphics
{
    struct RenderConfig;

    class RenderApi final : NonCopyable
    {
        Detail::RenderApi m_detail;
        Platform::Window* m_window = nullptr;
        DelegateHandle m_windowResizeDelegate;
        bool m_setup = false;

    public:
        RenderApi() = default;
        ~RenderApi();

        bool Setup(Platform::Window* window, const RenderConfig& config);
        void BeginFrame();
        void EndFrame();

    private:
        void OnResize(u32 width, u32 height);
    };
}
