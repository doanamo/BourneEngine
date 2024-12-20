#include "Shared.hpp"
#include "Platform/Thread.hpp"

void Platform::Thread::Sleep(u64 milliseconds)
{
    usleep(milliseconds * 1000);
}

void Platform::Thread::Pause()
{
    pause();
}

void Platform::Thread::Yield()
{
    sched_yield();
}
