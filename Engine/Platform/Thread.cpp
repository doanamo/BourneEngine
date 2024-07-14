#include "Shared.hpp"
#include "Thread.hpp"

void Platform::Sleep(u64 milliseconds)
{
#ifdef PLATFORM_WINDOWS
    ::Sleep(milliseconds);
#else
    #error Not implemented
#endif
}

void Platform::SleepForever()
{
    while(true)
    {
        Platform::Sleep(1000);
    }
}

void Platform::Yield()
{
#ifdef PLATFORM_WINDOWS
    ::SwitchToThread();
#else
    #error Not implemented
#endif
}
