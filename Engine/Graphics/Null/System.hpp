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
    class System final : NonCopyable
    {
    public:
        System() = default;
        ~System() = default;

        bool Setup(const Platform::Window* window, const SystemConfig& config);
        void Resize(u32 width, u32 height);
        void BeginFrame(u32 width, u32 height);
        void EndFrame();
    };
}
