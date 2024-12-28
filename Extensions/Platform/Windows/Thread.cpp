#include "Shared.hpp"
#include "Platform/Thread.hpp"
#include "Includes.hpp"

void Thread::Sleep(const u64 milliseconds)
{
    Win32::Sleep(milliseconds);
}

void Thread::Pause()
{
    Win32::Sleep(INFINITE);
}

void Thread::Yield()
{
    Win32::SwitchToThread();
}
