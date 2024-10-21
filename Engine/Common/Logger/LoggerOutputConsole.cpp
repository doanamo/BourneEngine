#include "Shared.hpp"
#include "LoggerOutputConsole.hpp"

void LoggerOutputConsole::Output(const char* text)
{
    // #todo: Remove call to this function entirely.
    // Also remove instantiation of this class.
#ifdef ENABLE_LOGGER_CONSOLE_OUTPUT
    printf(text);
#endif
}
