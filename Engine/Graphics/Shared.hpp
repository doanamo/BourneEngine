#pragma once

#define ENABLE_GRAPHICS_DEBUG CONFIG_DEBUG // Enable graphics debugging capabilities

#if defined(GRAPHICS_D3D11)
    #define GRAPHICS_NAME "Direct3D11"
    #include "Direct3D11/Shared.hpp"
#elif defined(GRAPHICS_NULL)
    #define GRAPHICS_NAME "Null"
    #include "Null/Shared.hpp"
#else
    #error "Unknown graphics"
#endif
