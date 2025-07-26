#include "Shared.hpp"
#include "RenderApi.hpp"
#include "Stats.hpp"
#include "Platform/Window.hpp"

Graphics::RenderApi::~RenderApi()
{
    if(m_window)
    {
        m_window->GetResizeDelegate().Remove(m_windowResizeDelegate);
    }

    if(m_setup)
    {
        LOG_DEBUG("Destroying graphics render API...");
    }
}

bool Graphics::RenderApi::Setup(Platform::Window* window, const RenderConfig& config)
{
    ASSERT(!m_setup);
    LOG_DEBUG("Setting up graphics render API...");

    ASSERT(window);
    m_window = window;

    if(!m_detail.Setup(m_window, config))
    {
        return false;
    }

    // #todo: Implement Function::CreateMethod() and similar utility functions for shortening this code.
    Function<void(u32 width, u32 height)> onResizeFunction;
    onResizeFunction.Bind<&Graphics::RenderApi::OnResize>(this);
    m_windowResizeDelegate = m_window->GetResizeDelegate().Add(std::move(onResizeFunction));

    LOG_SUCCESS("Graphics render API setup complete");
    return m_setup = true;
}

void Graphics::RenderApi::BeginFrame()
{
    m_detail.BeginFrame(m_window->GetWidth(), m_window->GetHeight());
}

void Graphics::RenderApi::EndFrame()
{
    m_detail.EndFrame();

    Stats& stats = Stats::Get();
    stats.OnEndFrame();
}

void Graphics::RenderApi::OnResize(u32 width, u32 height)
{
    m_detail.Resize(width, height);
}
