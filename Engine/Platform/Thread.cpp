#include "Shared.hpp"
#include "Platform/Thread.hpp"

void Platform::Sleep(u64 milliseconds)
{
    ::Sleep(milliseconds);
}

void Platform::SleepForever()
{
    ::Sleep(INFINITE);
}

void Platform::Yield()
{
    ::SwitchToThread();
}
