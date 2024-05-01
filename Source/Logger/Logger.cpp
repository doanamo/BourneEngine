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

void Logger::Log(const char* message, const char* source, u32 line)
{
    Debug::Print(message);
}
