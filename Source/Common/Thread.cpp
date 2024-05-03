#include "Common/Shared.hpp"
#include "Common/Thread.hpp"
#include "Common/Platform.hpp"

void Thread::Sleep(u64 milliseconds)
{
#ifdef PLATFORM_WINDOWS
    Sleep(milliseconds);
#else
    #error Not implemented
#endif
}

void Thread::Yield()
{
#ifdef PLATFORM_WINDOWS
    SwitchToThread();
#else
    #error Not implemented
#endif
}
