#pragma once

namespace Platform
{
    class Window;
}

namespace Graphics::Detail
{
    class System final : NonCopyable
    {
    public:
        System() = default;
        ~System() = default;

        bool Setup(const Platform::Window* window);
        void BeginFrame(const Platform::Window* window);
        void EndFrame();
    };
}
