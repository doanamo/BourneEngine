#include "Shared.hpp"
#include "Logger/Logger.hpp"

Logger& Logger::Get()
{
    static Logger instance;
    return instance;
}

void Logger::Log(const char* message, const char* source, u32 line)
{
    Debug::Print(message);
}
