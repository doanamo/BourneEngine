#include "Common/Debug.hpp"

#if defined(WIN32)
    #include <intrin.h>
#else
    #error Unsupported platform!
#endif

void Debug::Break()
{
    __debugbreak();
}

void Debug::Abort()
{
    __fastfail(7);
}
