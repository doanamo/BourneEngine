#pragma once

namespace Debug
{
    bool IsDebuggerPresent();
    void DebuggerPrint(const char* message);
}

#define DEBUG_BREAK() std::raise(SIGINT)
#define DEBUG_ABORT() std::raise(SIGABRT)