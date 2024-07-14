#include "Shared.hpp"
#include "Assert.hpp"
#include "Common/Logger/Logger.hpp"

static std::atomic<bool> g_handlingAssert = false;

void HandleAssert(const char* file, u32 line, const char* message, ...)
{
    if(g_handlingAssert.exchange(true))
    {
        // Already handling an assert
        Platform::SleepForever();
    }

    va_list arguments;
    va_start(arguments, message);

    LoggerMessage logMessage;
    logMessage.Format(message, arguments);
    logMessage.SetSeverity(LogSeverity::Fatal);
    logMessage.SetSource(file);
    logMessage.SetLine(line);
    Logger::Write(logMessage);

    va_end(arguments);
}
