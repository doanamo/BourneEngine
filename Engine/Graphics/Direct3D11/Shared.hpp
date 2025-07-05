#pragma once

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include <wrl.h>
using Microsoft::WRL::ComPtr;

#ifdef ENABLE_GRAPHICS_DEBUG
    #include <dxgidebug.h>
#endif
