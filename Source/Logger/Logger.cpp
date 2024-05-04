#include "Logger/Shared.hpp"
#include "Logger/Logger.hpp"
#include "LoggerFormat.hpp"

static LoggerFormat g_loggerFormat;
static std::mutex g_printMutex;

static void LogAssert(const char* expression, const char* message, const char* file, u32 line)
{
    LoggerMessage logMessage;
    if(message != nullptr)
    {
        logMessage.Format("Assertion failed: %s - %s", expression, message);
    }
    else
    {
        logMessage.Format("Assertion failed: %s", expression);
    }
    logMessage.SetSeverity(LogSeverity::Fatal);
    logMessage.SetSource(file);
    logMessage.SetLine(line);

    Logger::Write(logMessage);
}

void Logger::Setup()
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
}

void Logger::Write(const LoggerMessage& message)
{
    const char* epilogue = g_loggerFormat.FormatEpilogue(message);
    const char* prologue = g_loggerFormat.FormatPrologue(message);
    const char* text = message.GetText();

    {
        std::scoped_lock(g_printMutex);
        Debug::Print(epilogue);
        Debug::Print(text);
        Debug::Print(prologue);
    }

    if(message.IsFatal())
    {
        Debug::Abort();
    }
}
