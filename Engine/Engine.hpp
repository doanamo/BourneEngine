#pragma once

#include "Config.hpp"
#include "Application.hpp"
#include "Platform/Time.hpp"
#include "Platform/Window.hpp"
#include "Graphics/RenderApi.hpp"

class Engine final
{
    Time::Timer m_timer;
    Platform::Window m_window;
    Graphics::RenderApi m_renderApi;

    bool m_setupCalled = false;
    bool m_setupSucceeded = false;

public:
    Engine() = default;
    ~Engine();

    bool Setup(const Config& config);
    ExitCodes Run(Application& application);

    Platform::Window& GetWindow();
    Graphics::RenderApi& GetRenderApi();
};
