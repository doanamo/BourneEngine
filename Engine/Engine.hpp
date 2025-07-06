#pragma once

#include "Config.hpp"
#include "Application.hpp"
#include "Platform/Time.hpp"
#include "Platform/Window.hpp"
#include "Graphics/System.hpp"

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
    ExitCodes Run(Application& application);

    Platform::Window& GetWindow();
    Graphics::System& GetGraphics();
};
