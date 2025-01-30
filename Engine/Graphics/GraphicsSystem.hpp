#pragma once

#if defined(GRAPHICS_VULKAN)
    #include "Vulkan/GraphicsSystem.hpp"
#else
    #error "Unknown graphics define!"
#endif

namespace Platform
{
    class Window;
}

namespace Graphics
{
    class System final : NonCopyable
    {
        SystemPrivate m_private;
        Platform::Window* m_window = nullptr;

        struct PrivateConstructorTag
        {
            explicit PrivateConstructorTag() = default;
        };

    public:
        static UniquePtr<System> Create(Platform::Window* window);

        System(PrivateConstructorTag);
        ~System();

        void BeginFrame();
        void EndFrame();

    private:
        bool OnCreate();
        void OnDestroy();
    };
}
