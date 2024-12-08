#include "Shared.hpp"
#include "Logger.hpp"
#include "LoggerFormat.hpp"

void Logger::Write(const LoggerMessage& message)
{
    const char* text = LoggerFormat::Format(message);

    if(Debug::IsDebuggerPresent())
    {
        Debug::DebuggerPrint(text);
    }

#ifdef ENABLE_LOGGER_CONSOLE_OUTPUT
    printf("%s", text);
#endif
}
