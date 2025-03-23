#pragma once

#if defined(GRAPHICS_VULKAN)
    #define GRAPHICS_NAME "Vulkan"
#else
    #error "Unknown graphics"
#endif

#define ENABLE_GRAPHICS_DEBUG CONFIG_DEBUG // Enable graphics debugging capabilities
