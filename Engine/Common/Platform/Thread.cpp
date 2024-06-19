#include "Shared.hpp"
#include "Thread.hpp"

void Thread::Sleep(u64 milliseconds)
{
#ifdef PLATFORM_WINDOWS
    ::Sleep(milliseconds);
#else
    #error Not implemented
#endif
}

void Thread::SleepForever()
{
    while(true)
    {
        Thread::Sleep(1000);
    }
}

void Thread::Yield()
{
#ifdef PLATFORM_WINDOWS
    ::SwitchToThread();
#else
    #error Not implemented
#endif
}
