#include "Logger/Shared.hpp"
#include "Logger/Logger.hpp"
#include "LoggerFormat.hpp"

static LoggerFormat g_loggerFormat;

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
    const char* epilogue = g_loggerFormat.FormatEpilogue(message);
    const char* prologue = g_loggerFormat.FormatPrologue(message);
    const char* text = message.GetText();

    Debug::Print(epilogue);
    Debug::Print(text);
    Debug::Print(prologue);

    if(message.IsFatal())
    {
        Debug::Abort();
    }
}
