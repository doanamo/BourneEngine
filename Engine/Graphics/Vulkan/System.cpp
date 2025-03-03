#include "Shared.hpp"
#include "System.hpp"
#include "Graphics/System.hpp"

bool Graphics::System::OnSetup()
{
    if(!m_detail.instance.Setup())
        return false;

    return true;
}

void Graphics::System::OnDestroy()
{

}
