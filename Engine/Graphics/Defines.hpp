#pragma once

#ifdef GRAPHICS_API_DIRECT3D11
    #define GRAPHICS_API_NAME "Direct3D11"
#elif GRAPHICS_API_NULL
    #define GRAPHICS_API_NAME "Null"
#else
    #error Unknown graphics API!
#endif

#ifdef CONFIG_DEBUG
    #define ENABLE_GRAPHICS_DEBUG // Enable graphics debugging
#endif
