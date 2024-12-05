#pragma once

#include "GraphicsStats.hpp"

namespace Platform
{
    class Window;
    class TimeSlice;
}

namespace Graphics
{
    class System final
    {
        const Platform::Window* m_window = nullptr;
        Stats m_stats;

    public:
        System() = default;
        ~System();

        System(const System&) = delete;
        System& operator=(const System&) = delete;

        bool Setup(const Platform::Window* window);
        void BeginFrame();
        void EndFrame();

        const Stats& GetStats() const
        {
            return m_stats;
        }
    };
}
