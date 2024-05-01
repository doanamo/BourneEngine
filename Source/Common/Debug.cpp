#include "Common/Debug.hpp"
#include "Common/Defines.hpp"

#if defined(PLATFORM_WINDOWS)
    #include <intrin.h>
#endif

void Debug::Break()
{
#if defined(PLATFORM_WINDOWS)
    __debugbreak();
#else
    #error Not implemented
#endif
}

void Debug::Abort()
{
#if defined(PLATFORM_WINDOWS)
    __fastfail(7);
#else
    #error Not implemented
#endif
}
