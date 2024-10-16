#pragma once

#include "Common/Defines.hpp"

namespace Debug
{
    bool IsDebuggerPresent();
    void Print(const char* message);
}

#if defined(PLATFORM_WINDOWS)
    #define DEBUG_BREAK() __debugbreak()
    #define DEBUG_ABORT() __fastfail(7)
#else
    #error Not implemented
#endif
