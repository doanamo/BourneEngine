#include "Shared.hpp"
#include "Platform/Thread.hpp"
#include "Includes.hpp"

void Thread::Sleep(const u64 milliseconds)
{
    usleep(milliseconds * 1000);
}

void Thread::Pause()
{
    pause();
}

void Thread::Yield()
{
    sched_yield();
}
