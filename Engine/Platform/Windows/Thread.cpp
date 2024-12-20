#include "Shared.hpp"
#include "Platform/Thread.hpp"

void Platform::Thread::Sleep(u64 milliseconds)
{
    ::Sleep(milliseconds);
}

void Platform::Thread::Pause()
{
    ::Sleep(INFINITE);
}

void Platform::Thread::Yield()
{
    ::SwitchToThread();
}
