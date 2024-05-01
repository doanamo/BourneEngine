#include "Logger/Shared.hpp"
#include "Logger/Logger.hpp"

static void LogAssert(const char* expression, const char* file, u32 line, const char* message)
{
    LOG("Assertion failed!");
}

void Logger::Setup()
{
    SetAssertCallback(&LogAssert);
}

void Logger::Write(const LoggerMessage& message)
{
    Debug::Print(message.GetText());
}
