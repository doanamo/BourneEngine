#include "Shared.hpp"
#include "Assert.hpp"

#if ENABLE_LOGGER
    #include "Common/Logger/Logger.hpp"
    #include "Common/Logger/Message.hpp"
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

    Logger::Message logMessage;
    logMessage.FormatArguments(message, arguments);
    logMessage.SetSeverity(Logger::Severity::Fatal);
    logMessage.SetSource(file);
    logMessage.SetLine(line);
    Logger::Write(logMessage);

    va_end(arguments);
#endif
}
