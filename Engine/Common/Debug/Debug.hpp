#pragma once

#include "Common/Defines.hpp"

namespace Debug
{
    bool IsDebuggerPresent();
    void DebuggerPrint(const char* message);
}

#define DEBUG_BREAK() __debugbreak()
#define DEBUG_ABORT() __fastfail(7)
