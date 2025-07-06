#include "Shared.hpp"
#include "System.hpp"
#include "Graphics/System.hpp"

bool Graphics::Detail::System::Setup(const Platform::Window* window, const SystemConfig& config)
{
    return true;
}

void Graphics::Detail::System::BeginFrame(const Platform::Window* window)
{
}

void Graphics::Detail::System::EndFrame()
{
}
