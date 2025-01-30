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
    // #todo: Create type safe version of printf so there is no need to remember format specifiers.
    //        For example allow LOG("Hello {}!", "World"). Also, this would allow seamless handling of
    //        StringView printing which otherwise would requre following format:
    //        > printf("=%.*s=", static_cast<int>(view.length()), view.data());
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
