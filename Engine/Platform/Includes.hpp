#pragma once

#if defined(PLATFORM_WINDOWS)
    #include "Windows/Includes.hpp"
#elif defined(PLATFORM_LINUX)
    #include "Linux/Includes.hpp"
#else
    #error "Unknown platform define"
#endif

namespace Platform
{
    using OnWindowCloseFunction = Function<void()>;
    using OnWindowResizeFunction = Function<void(u32 width, u32 height)>;
}
