#include "Shared.hpp"
#include "Platform/Thread.hpp"

void Platform::Sleep(u64 milliseconds)
{
#if defined(PLATFORM_WINDOWS)
    ::Sleep(milliseconds);
#elif defined(PLATFORM_LINUX)
    usleep(milliseconds * 1000);
#else
    #error Unknown platform!
#endif
}

void Platform::SleepForever()
{
#if defined(PLATFORM_WINDOWS)
    ::Sleep(INFINITE);
#elif defined(PLATFORM_LINUX)
    pause();
#else
    #error Unknown platform!
#endif
}

void Platform::Yield()
{
#if defined(PLATFORM_WINDOWS)
    ::SwitchToThread();
#elif defined(PLATFORM_LINUX)
    sched_yield();
#else
    #error Unknown platform!
#endif
}
