#include "Shared.hpp"
#include "LoggerOutputConsole.hpp"

void LoggerOutputConsole::Output(const char* text)
{
#ifdef LOGGER_CONSOLE_OUTPUT
    printf(text);
#endif
}
