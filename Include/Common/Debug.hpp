#pragma once

namespace Debug
{
    void Break();
    void Abort();

    void DebuggerPrint(const char* message);
    bool IsDebuggerPresent();
}
