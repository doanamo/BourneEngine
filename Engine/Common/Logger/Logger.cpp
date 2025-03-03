#include "Shared.hpp"
#include "Logger.hpp"
#include "Format.hpp"

#if ENABLE_LOGGER

void Logger::Write(const Message& message)
{
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
        break;

    default:
        fprintf(stdout, "%s", text);
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
