#include "Shared.hpp"
#include "RenderApi.hpp"
#include "Graphics/RenderApi.hpp"

bool Graphics::Detail::RenderApi::Setup(const Platform::Window* window, const RenderConfig& config)
{
    return true;
}

void Graphics::Detail::RenderApi::Resize(u32 width, u32 height)
{
}

void Graphics::Detail::RenderApi::BeginFrame(u32 width, u32 height)
{
}

void Graphics::Detail::RenderApi::EndFrame()
{
}
