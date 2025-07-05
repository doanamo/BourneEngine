#include "Shared.hpp"
#include "Platform/Debug.hpp"

bool Debug::IsDebuggerPresent()
{
    bool debuggerPresent = false;
    int statusFile = open("/proc/self/status", O_RDONLY);
    if(statusFile >= 0)
    {
        char buffer[256];
        const u64 bytesRead = read(statusFile, buffer, sizeof(buffer));

        const char* tracerText = "TracerPid:\t";
        const u32 tracerTextLength = std::strlen(tracerText);

        for(u64 i = 0; i < bytesRead - tracerTextLength - 1; i++)
        {
            if(strncmp(&buffer[i], tracerText, tracerTextLength) == 0)
            {
                debuggerPresent = buffer[i + tracerTextLength] != '0';
                break;
            }
        }

        close(statusFile);
    }

    return debuggerPresent;
}

void Debug::DebuggerPrint(const char* message)
{
    // There is no way to print a message to the debugger on Linux. Instead, we log to stdout and stderr.
}
