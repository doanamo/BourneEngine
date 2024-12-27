#include "Shared.hpp"
#include "LoggerMessage.hpp"

#ifdef ENABLE_LOGGER

thread_local char LoggerMessage::m_buffer[];

LoggerMessage::LoggerMessage()
{
    m_buffer[0] = '\0';
}

LoggerMessage& LoggerMessage::Format(const char* format, std::va_list arguments)
{
    ASSERT_EVALUATE(std::vsprintf(m_buffer, format, arguments) >= 0, "Failed to format message");
    return *this;
}

LoggerMessage& LoggerMessage::Format(const char* format, ...)
{
    std::va_list arguments;
    va_start(arguments, format);
    Format(format, arguments);
    va_end(arguments);
    return *this;
}

#endif
