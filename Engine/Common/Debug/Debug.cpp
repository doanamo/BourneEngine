#include "Precompiled.hpp"
#include "Debug.hpp"

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

bool Debug::IsDebuggerPresent()
{
#if defined(PLATFORM_WINDOWS)
    return ::IsDebuggerPresent();
#else
    #error Not implemented
#endif
}

void Debug::Print(const char* message)
{
#if defined(PLATFORM_WINDOWS)
    // This call is thread safe, but does not guarantee that the message will be
    // printed in order when called from different threads. Implementing a custom
    // locking mechanism does not guarantee that the message will be printed in
    // order either, so it is not worth the effort.
    ::OutputDebugStringA(message);
#else
    #error Not implemented
#endif
}
