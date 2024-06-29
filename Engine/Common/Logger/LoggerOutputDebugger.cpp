#include "Shared.hpp"
#include "LoggerOutputDebugger.hpp"

void LoggerOutputDebugger::Output(const char* text)
{
    if(Debug::IsDebuggerPresent())
    {
        Debug::Print(text);
    }
}
