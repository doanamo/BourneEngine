#include "Logger/Shared.hpp"
#include "Logger/Logger.hpp"

static bool g_loggingAssert = false;

static void LogAssert(const char* expression, const char* file, u32 line, const char* message)
{
    g_loggingAssert = true;

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

    g_loggingAssert = false;
}

void Logger::Setup()
{
    SetAssertCallback(&LogAssert);
}

void Logger::Write(const LoggerMessage& message)
{
    Debug::Print(message.GetText());

    if(message.IsFatal() && !g_loggingAssert)
    {
        Debug::Abort();
    }
}
