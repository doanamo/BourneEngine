#pragma once

#if defined(GRAPHICS_D3D11)
    #define GRAPHICS_NAME "Direct3D11"
#elif defined(GRAPHICS_NULL)
    #define GRAPHICS_NAME "Null"
#else
    #error "Unknown graphics"
#endif

#define ENABLE_GRAPHICS_DEBUG CONFIG_DEBUG // Enable graphics debugging capabilities
