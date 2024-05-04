#include "Logger/Shared.hpp"
#include "LoggerOutputDebugger.hpp"

LoggerOutputDebugger::LoggerOutputDebugger()
    : m_isDebuggerPresent(Debug::IsDebuggerPresent())
{
}

void LoggerOutputDebugger::Output(const char* text)
{
    if(m_isDebuggerPresent)
    {
        Debug::DebuggerPrint(text);
    }
}
