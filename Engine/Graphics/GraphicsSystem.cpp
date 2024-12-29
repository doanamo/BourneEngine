#include "Shared.hpp"
#include "GraphicsSystem.hpp"
#include "GraphicsStats.hpp"
#include "Platform/Window.hpp"

Graphics::System::~System()
{
    LOG("Destroying graphics system...");
}

bool Graphics::System::Setup(const Platform::Window* window)
{
    LOG("Creating graphics system...");

    ASSERT(window);
    m_window = window;

    LOG_SUCCESS("Created graphics system");
    return true;
}

void Graphics::System::BeginFrame()
{
}

void Graphics::System::EndFrame()
{
    m_stats.OnEndFrame();
}
