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
#if defined(PLATFORM_LINUX)
    #define DEBUG_BREAK() __builtin_trap()
    #define DEBUG_ABORT() _exit(2)
#else
    #error Not implemented
#endif
