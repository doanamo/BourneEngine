#include "Shared.hpp"
#include "LoggerFormat.hpp"
#include "LoggerMessage.hpp"
#include <ctime>

#ifdef ENABLE_LOGGER

static thread_local char t_loggerFormatBuffer[
    LoggerMessage::FormatBufferSize + 1024];

static const char* GetLogSeverityName(const LogSeverity severity)
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

    ASSERT_SLOW(false, "Invalid log severity");
    return "Invalid";
}

static const char* ParseLogSourcePath(const char* source)
{
    if(const char* matchBegin = strstr(source, BuildInfo::ProjectPath))
    {
        return matchBegin + strlen(BuildInfo::ProjectPath);
    }

    return source;
}

const char* LoggerFormat::Format(const LoggerMessage& message)
{
    t_loggerFormatBuffer[0] = '\0';

    const std::time_t time = std::time(nullptr);
    const std::tm* now = std::localtime(&time);

    char timeBuffer[128] = { 0 };
    ASSERT_EVALUATE(std::strftime(timeBuffer, ArraySize(timeBuffer),
        "%Y-%m-%d %H:%M:%S %z", now) > 0, "Failed to format time");

#ifdef ENABLE_LOGGER_SOURCE_LINE
    ASSERT_EVALUATE(std::snprintf(t_loggerFormatBuffer, ArraySize(t_loggerFormatBuffer),
        "[%s][%-7s] %s {%s:%u}\n", timeBuffer, GetLogSeverityName(message.GetSeverity()),
        message.GetText(), ParseLogSourcePath(message.GetSource()), message.GetLine()) >= 0,
        "Failed to format epilogue");
#else
    ASSERT_EVALUATE(std::snprintf(t_loggerFormatBuffer, ArraySize(t_loggerFormatBuffer),
        "[%s][%-7s] %s\n", timeBuffer, GetLogSeverityName(message.GetSeverity()),
        message.GetText()) >= 0, "Failed to format epilogue");
#endif

    return t_loggerFormatBuffer;
}

#endif
