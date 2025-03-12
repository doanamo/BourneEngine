#include "Shared.hpp"
#include "Message.hpp"

#if ENABLE_LOGGER

thread_local char Logger::Message::t_buffer[];

Logger::Message::Message()
{
    t_buffer[0] = '\0';
}

Logger::Message& Logger::Message::Format(const char* format, std::va_list arguments)
{
    ASSERT_EVALUATE(std::vsprintf(t_buffer, format, arguments) >= 0, "Failed to format message");
    return *this;
}

Logger::Message& Logger::Message::Format(const char* format, ...)
{
    std::va_list arguments;
    va_start(arguments, format);
    Format(format, arguments);
    va_end(arguments);
    return *this;
}

#endif
