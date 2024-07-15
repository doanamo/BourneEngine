#pragma once

#ifdef PLATFORM_WINDOWS
    #define GRAPHICS_DIRECT3D12
    #define GRAPHICS_NAME "Direct3D12"
#else
    #error Unsupported platform!
#endif
