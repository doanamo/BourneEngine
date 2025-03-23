#pragma once

#if defined(GRAPHICS_VULKAN)
    #include "Vulkan/Includes.hpp"
#else
    #error "Unknown graphics define"
#endif
