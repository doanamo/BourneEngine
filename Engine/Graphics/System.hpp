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
        void OnBeginFrame();
        void OnEndFrame();
    };
}
