#include "Shared.hpp"
#include "Logger.hpp"
#include "LoggerFormat.hpp"
#include "LoggerOutputDebugger.hpp"
#include "LoggerOutputConsole.hpp"

static LoggerOutputDebugger g_loggerOutputDebugger;
static LoggerOutputConsole g_loggerOutputConsole;

void Logger::Setup()
{
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
    const char* text = LoggerFormat::Format(message);

    g_loggerOutputDebugger.Output(text);
    g_loggerOutputConsole.Output(text);
}
