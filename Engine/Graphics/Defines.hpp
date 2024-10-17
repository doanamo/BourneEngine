#pragma once

#ifdef CONFIG_DEBUG
    #define ENABLE_GRAPHICS_DEBUG // Enable graphics debugging
#endif

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#ifdef ENABLE_GRAPHICS_DEBUG
    #include <dxgidebug.h>
#endif

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
