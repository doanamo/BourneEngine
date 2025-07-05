#include "Shared.hpp"
#include "Logger.hpp"
#include "Format.hpp"

#if ENABLE_LOGGER

Logger::Severity Logger::g_minimumSeverity = Severity::Info;
thread_local bool Logger::t_writeSourceLine = true;

void Logger::Write(const Message& message)
{
    if(message.GetSeverity() < g_minimumSeverity)
        return;

    const char* text = Format(message);

    if(Debug::IsDebuggerPresent())
    {
        Debug::DebuggerPrint(text);
    }

#if ENABLE_LOGGER_CONSOLE_OUTPUT
    switch(message.GetSeverity())
    {
    case Severity::Fatal:
    case Severity::Error:
    case Severity::Warning:
        fprintf(stderr, "%s", text);
        fflush(stderr);
        break;

    default:
        fprintf(stdout, "%s", text);
        fflush(stdout);
    }
#endif
}

void Logger::Flush()
{
#if ENABLE_LOGGER_CONSOLE_OUTPUT
    fflush(stdout);
    fflush(stderr);
#endif
}

#endif
