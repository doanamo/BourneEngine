#pragma once

namespace Platform
{
    class Window;
}

namespace Graphics
{
    struct SystemConfig;
}

namespace Graphics::Detail
{
    class RenderApi final : NonCopyable
    {
    public:
        RenderApi() = default;
        ~RenderApi() = default;

        bool Setup(const Platform::Window* window, const RenderConfig& config);
        void Resize(u32 width, u32 height);
        void BeginFrame(u32 width, u32 height);
        void EndFrame();
    };
}
