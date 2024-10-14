#pragma once

#ifdef PLATFORM_WINDOWS
    #define GRAPHICS_DIRECT3D11
    #define GRAPHICS_NAME "Direct3D11"
#else
    #error Unsupported platform!
#endif

#ifdef CONFIG_DEBUG
    #define ENABLE_GRAPHICS_DEBUG // Enable graphics debugging
#endif
