#include "Shared.hpp"
#include "GraphicsSystem.hpp"
#include "Graphics/GraphicsSystem.hpp"

bool Graphics::System::OnSetup()
{
    if(!m_detail.instance.Setup())
        return false;

    return true;
}

void Graphics::System::OnDestroy()
{

}
