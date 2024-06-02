#pragma once

namespace Debug
{
    void Break();
    void Abort();

    bool IsDebuggerPresent();
    void Print(const char* message);
}
