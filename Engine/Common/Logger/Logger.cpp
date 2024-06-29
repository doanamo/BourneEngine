#include "Shared.hpp"
#include "Logger.hpp"
#include "LoggerFormat.hpp"
#include "LoggerOutputDebugger.hpp"
#include "LoggerOutputConsole.hpp"

static LoggerOutputDebugger g_loggerOutputDebugger;
static LoggerOutputConsole g_loggerOutputConsole;

void Logger::Write(const LoggerMessage& message)
{
    const char* text = LoggerFormat::Format(message);

    g_loggerOutputDebugger.Output(text);
    g_loggerOutputConsole.Output(text);
}
