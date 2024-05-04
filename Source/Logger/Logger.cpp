#include "Logger/Shared.hpp"
#include "Logger/Logger.hpp"
#include "LoggerFormat.hpp"

static void LogAssert(const char* file, u32 line, const char* message, va_list arguments)
{
    LoggerMessage logMessage;
    logMessage.Format(message, arguments);
    logMessage.SetSeverity(LogSeverity::Fatal);
    logMessage.SetSource(file);
    logMessage.SetLine(line);

    Logger::Write(logMessage);
}

bool Logger::Setup()
{
    SetAssertCallback(&LogAssert);

#if 0
    LOG_DEBUG("Example debug message");
    LOG_INFO("Example info message");
    LOG_SUCCESS("Example success message");
    LOG_WARNING("Example warning message");
    LOG_ERROR("Example error message");
    LOG_FATAL("Example fatal message");
#endif

    return true;
}

void Logger::Write(const LoggerMessage& message)
{
    Debug::Print(LoggerFormat::Format(message));

    if(message.IsFatal())
    {
        Debug::Abort();
    }
}
