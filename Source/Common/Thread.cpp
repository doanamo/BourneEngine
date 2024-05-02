#include "Common/Shared.hpp"
#include "Common/Thread.hpp"
#include <thread>

void Thread::Sleep(u64 milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void Thread::Yield()
{
    std::this_thread::yield();
}
