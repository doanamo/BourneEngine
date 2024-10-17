#pragma once

#ifdef GRAPHICS_DIRECT3D11
    #define GRAPHICS_NAME "Direct3D11"
#elif GRAPHICS_NULL
    #define GRAPHICS_NAME "Null"
#else
    #error Unsupported graphics API
#endif

#ifdef CONFIG_DEBUG
    #define ENABLE_GRAPHICS_DEBUG // Enable graphics debugging
#endif
