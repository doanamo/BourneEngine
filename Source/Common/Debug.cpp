#include "Common/Debug.hpp"
#include "Common/Defines.hpp"

#if defined(PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

void Debug::Print(const char* message)
{
#if defined(PLATFORM_WINDOWS)
    OutputDebugStringA(message);
    OutputDebugStringA("\n");
#else
    #error Not implemented
#endif
}

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
