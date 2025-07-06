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
        void BeginFrame(const Platform::Window* window);
        void EndFrame();
    };
}
