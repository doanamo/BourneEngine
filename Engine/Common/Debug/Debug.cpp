#include "Shared.hpp"
#include "Debug.hpp"

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
