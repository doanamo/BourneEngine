#include "Shared.hpp"
#include "Assert.hpp"

#if ENABLE_LOGGER
    #include "Common/Logger/Logger.hpp"
    #include "Common/Logger/LoggerMessage.hpp"
#endif

static std::atomic<bool> g_handlingAssert = false;

void HandleAssert(const char* file, const u32 line, const char* message, ...)
{
    if(g_handlingAssert.exchange(true))
    {
        // Already handling assert
        Thread::Pause();
    }

#if ENABLE_LOGGER
    std::va_list arguments;
    va_start(arguments, message);

    LoggerMessage logMessage;
    logMessage.Format(message, arguments);
    logMessage.SetSeverity(LogSeverity::Fatal);
    logMessage.SetSource(file);
    logMessage.SetLine(line);
    Logger::Write(logMessage);
    Logger::Flush();

    va_end(arguments);
#endif
}
