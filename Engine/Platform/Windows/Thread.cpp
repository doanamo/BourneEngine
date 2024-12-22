#include "Shared.hpp"
#include "Platform/Thread.hpp"

void Thread::Sleep(const u64 milliseconds)
{
    ::Sleep(milliseconds);
}

void Thread::Pause()
{
    ::Sleep(INFINITE);
}

void Thread::Yield()
{
    ::SwitchToThread();
}
