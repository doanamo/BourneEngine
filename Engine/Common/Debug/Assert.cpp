#include "Shared.hpp"
#include "Assert.hpp"

#if !defined(CONFIG_RELEASE)
#include "Common/Logger/Logger.hpp"
#include "Common/Logger/LoggerMessage.hpp"
#endif

static std::atomic<bool> g_handlingAssert = false;

void HandleAssert(const char* file, u32 line, const char* message, ...)
{
    if(g_handlingAssert.exchange(true))
    {
        // Already handling an assert
        Platform::SleepForever();
    }

#if !defined(CONFIG_RELEASE)
    va_list arguments;
    va_start(arguments, message);

    LoggerMessage logMessage;
    logMessage.Format(message, arguments);
    logMessage.SetSeverity(LogSeverity::Fatal);
    logMessage.SetSource(file);
    logMessage.SetLine(line);
    Logger::Write(logMessage);

    va_end(arguments);
#endif
}
