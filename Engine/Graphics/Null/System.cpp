#include "Shared.hpp"
#include "System.hpp"
#include "Graphics/System.hpp"

bool Graphics::Detail::System::Setup(const Platform::Window* window, const SystemConfig& config)
{
    return true;
}

void Graphics::Detail::System::Resize(u32 width, u32 height)
{
}

void Graphics::Detail::System::BeginFrame(u32 width, u32 height)
{
}

void Graphics::Detail::System::EndFrame()
{
}
