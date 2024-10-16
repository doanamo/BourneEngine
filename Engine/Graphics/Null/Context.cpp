#include "Shared.hpp"
#include "Graphics/Context.hpp"

Graphics::Context::~Context()
{
}

bool Graphics::Context::Setup(const Platform::Window& window)
{
    return true;
}

void Graphics::Context::BeginFrame(const Platform::Window& window)
{
}

void Graphics::Context::EndFrame()
{
}
