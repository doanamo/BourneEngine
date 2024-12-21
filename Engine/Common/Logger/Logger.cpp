#include "Shared.hpp"
#include "Logger.hpp"
#include "LoggerFormat.hpp"

void Logger::Write(const LoggerMessage& message)
{
    const char* text = LoggerFormat::Format(message);

    if(Debug::IsDebuggerPresent())
    {
        Debug::DebuggerPrint(text);
    }

#ifdef ENABLE_LOGGER_CONSOLE_OUTPUT
    switch(message.GetSeverity())
    {
    case LogSeverity::Fatal:
    case LogSeverity::Error:
    case LogSeverity::Warning:
        fprintf(stderr, "%s", text);
        break;

    default:
        fprintf(stdout, "%s", text);
    }
#endif
}

void Logger::Flush()
{
#ifdef ENABLE_LOGGER_CONSOLE_OUTPUT
    fflush(stdout);
    fflush(stderr);
#endif
}
