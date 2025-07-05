#pragma once

#include "Config.hpp"
#include "Platform/Time.hpp"
#include "Platform/Window.hpp"
#include "Graphics/System.hpp"

namespace Engine
{
    class Engine final
    {
        Time::Timer m_timer;
        Platform::Window m_window;
        Graphics::System m_graphics;

        bool m_setupCalled = false;
        bool m_setupSucceeded = false;

    public:
        Engine() = default;
        ~Engine();

        bool Setup(const Config& config);
        int Run();

        Platform::Window& GetWindow();
        Graphics::System& GetGraphics();

    private:
        void LogPreSetupInfo();
        void ParseCommandLine(const Config& config);
    };

    const char* GetApplicationName();
}
