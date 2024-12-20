#pragma once

namespace Platform::Thread
{
    void Sleep(u64 milliseconds);
    void Pause();
    void Yield();
}
