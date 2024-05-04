#include "Logger/Shared.hpp"
#include "LoggerOutputConsole.hpp"

void LoggerOutputConsole::Output(const char* text)
{
#ifndef CONFIG_RELEASE
    printf(text);
#endif
}
