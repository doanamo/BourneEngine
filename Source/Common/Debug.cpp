#include "Common/Shared.hpp"
#include "Common/Debug.hpp"
#include "Common/Platform.hpp"

void Debug::Print(const char* message)
{
#if defined(PLATFORM_WINDOWS)
    OutputDebugStringA(message);
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

bool Debug::IsDebuggerAttached()
{
#if defined(PLATFORM_WINDOWS)
    return IsDebuggerPresent();
#else
    #error Not implemented
#endif
}
