#include "Logger/Shared.hpp"
#include "Logger/LoggerMessage.hpp"

thread_local char LoggerMessage::m_buffer[];

LoggerMessage::LoggerMessage()
{
    m_buffer[0] = '\0';
}

LoggerMessage& LoggerMessage::Format(const char* format, va_list arguments)
{
    ASSERT_EVALUATE(vsprintf_s(m_buffer, StaticArraySize(m_buffer),
        format, arguments) >= 0, "Failed to format message");
    return *this;
}

LoggerMessage& LoggerMessage::Format(const char* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    Format(format, arguments);
    va_end(arguments);
    return *this;
}
