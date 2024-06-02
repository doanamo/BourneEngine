#pragma once

#include "Common/Types.hpp"

namespace Thread
{
    void Sleep(u64 milliseconds);
    void SleepForever();
    void Yield();
}
