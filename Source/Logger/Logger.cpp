#include "Logger/Shared.hpp"
#include "Logger/Logger.hpp"

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
}

void Logger::Write(const LoggerMessage& message)
{
    Debug::Print(message.GetText());

    if(message.IsFatal())
    {
        Debug::Abort();
    }
}
