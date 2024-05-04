#include "Logger/Shared.hpp"
#include "LoggerFormat.hpp"
#include <ctime>

static thread_local char m_loggerFormatBuffer[1024 * 5];

static const char* GetLogSeverityName(LogSeverity severity)
{
    switch(severity)
    {
    case LogSeverity::Debug:    return "Debug";
    case LogSeverity::Info:     return "Info";
    case LogSeverity::Success:  return "Success";
    case LogSeverity::Warning:  return "Warning";
    case LogSeverity::Error:    return "Error";
    case LogSeverity::Fatal:    return "Fatal";
    }

    ASSERT(false, "Invalid log severity");
    return "Invalid";
}

const char* LoggerFormat::Format(const LoggerMessage& message)
{
    m_loggerFormatBuffer[0] = '\0';

    std::time_t time = std::time(nullptr);
    std::tm* now = std::localtime(&time);

    char timeBuffer[64] = { 0 };
    ASSERT_EVALUATE(std::strftime(timeBuffer, StaticArraySize(timeBuffer),
        "%Y-%m-%d %H:%M:%S %z", now) >= 0, "Failed to format time");

#ifdef CONFIG_DEBUG
    const char* source = message.GetSource();
    const char* sourceBegin = strstr(source, "Source\\");
    if(sourceBegin == nullptr)
    {
        sourceBegin = source;
    }

    ASSERT_EVALUATE(snprintf(m_loggerFormatBuffer, StaticArraySize(m_loggerFormatBuffer),
        "[%s][%-7s] %s {%s:%u}\n", timeBuffer, GetLogSeverityName(message.GetSeverity()),
        message.GetText(), sourceBegin, message.GetLine()) >= 0,
        "Failed to format epilogue");
#else
    ASSERT_EVALUATE(snprintf(m_loggerFormatBuffer, StaticArraySize(m_loggerFormatBuffer),
        "[%s][%-7s] %s\n", timeBuffer, GetLogSeverityName(message.GetSeverity()),
        message.GetText()) >= 0, "Failed to format epilogue");
#endif

    return m_loggerFormatBuffer;
}
