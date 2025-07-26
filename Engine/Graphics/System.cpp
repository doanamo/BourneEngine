#include "Shared.hpp"
#include "System.hpp"
#include "Stats.hpp"
#include "Platform/Window.hpp"

Graphics::System::~System()
{
    if(m_window)
    {
        m_window->GetResizeDelegate().Remove(m_windowResizeDelegate);
    }

    if(m_setup)
    {
        LOG_DEBUG("Destroying graphics...");
    }
}

bool Graphics::System::Setup(Platform::Window* window, const SystemConfig& config)
{
    ASSERT(!m_setup);
    LOG_DEBUG("Setting up graphics...");

    ASSERT(window);
    m_window = window;

    if(!m_detail.Setup(m_window, config))
    {
        LOG_ERROR("Failed to setup graphics");
        return false;
    }

    // #todo: Implement Function::CreateMethod() and similar utility functions for shortening this code.
    Function<void(u32 width, u32 height)> onResizeFunction;
    onResizeFunction.Bind<&Graphics::System::OnResize>(this);
    m_windowResizeDelegate = m_window->GetResizeDelegate().Add(std::move(onResizeFunction));

    LOG_SUCCESS("Graphics setup complete");
    return m_setup = true;
}

void Graphics::System::BeginFrame()
{
    m_detail.BeginFrame(m_window->GetWidth(), m_window->GetHeight());
}

void Graphics::System::EndFrame()
{
    m_detail.EndFrame();

    Stats& stats = Stats::Get();
    stats.OnEndFrame();
}

void Graphics::System::OnResize(u32 width, u32 height)
{
    m_detail.Resize(width, height);
}
