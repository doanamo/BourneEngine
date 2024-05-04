#include "Logger/Shared.hpp"
#include "Logger/Logger.hpp"
#include "LoggerFormat.hpp"
#include "LoggerOutputDebugger.hpp"
#include "LoggerOutputConsole.hpp"

static LoggerOutputDebugger g_loggerOutputDebugger;
static LoggerOutputConsole g_loggerOutputConsole;

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
    const char* text = LoggerFormat::Format(message);

    g_loggerOutputDebugger.Output(text);
    g_loggerOutputConsole.Output(text);

    if(message.IsFatal())
    {
        Debug::Abort();
    }
}
