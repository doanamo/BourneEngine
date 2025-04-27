#pragma once

#if defined(GRAPHICS_D3D11)
    #include "Direct3D11/Includes.hpp"
#elif defined(GRAPHICS_NULL)
    #include "Null/Includes.hpp"
#else
    #error "Unknown graphics"
#endif
