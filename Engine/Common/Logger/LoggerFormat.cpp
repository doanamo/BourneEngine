#include "Shared.hpp"
#include "LoggerFormat.hpp"
#include <ctime>

static thread_local char m_loggerFormatBuffer[
    LoggerMessage::FormatBufferSize + 1024];

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

static const char* ParseLogSourcePath(const char* source)
{
    constexpr char sourceDir[] = "\\Source\\";
    constexpr u64 sourceOffset = ArraySize(sourceDir) - 1;

    constexpr char includeDir[] = "\\Include\\";
    constexpr u64 includeOffset = ArraySize(includeDir) - 1;

    constexpr char testsDir[] = "\\Tests\\";
    constexpr u64 testsOffset = ArraySize(testsDir) - 1;

    const char* matchBegin = strstr(source, &sourceDir[0]);
    if(matchBegin != nullptr)
    {
        return matchBegin + sourceOffset;
    }

    matchBegin = strstr(source, &includeDir[0]);
    if(matchBegin != nullptr)
    {
        return matchBegin + includeOffset;
    }

    matchBegin = strstr(source, &testsDir[0]);
    if(matchBegin != nullptr)
    {
        return matchBegin + testsOffset;
    }

    return source;
}

const char* LoggerFormat::Format(const LoggerMessage& message)
{
    m_loggerFormatBuffer[0] = '\0';

    std::time_t time = std::time(nullptr);
    std::tm* now = std::localtime(&time);

    char timeBuffer[64] = { 0 };
    ASSERT_EVALUATE(std::strftime(timeBuffer, ArraySize(timeBuffer),
        "%Y-%m-%d %H:%M:%S %z", now) >= 0, "Failed to format time");

#ifdef CONFIG_DEBUG
    ASSERT_EVALUATE(snprintf(m_loggerFormatBuffer, ArraySize(m_loggerFormatBuffer),
        "[%s][%-7s] %s {%s:%u}\n", timeBuffer, GetLogSeverityName(message.GetSeverity()),
        message.GetText(), ParseLogSourcePath(message.GetSource()), message.GetLine()) >= 0,
        "Failed to format epilogue");
#else
    ASSERT_EVALUATE(snprintf(m_loggerFormatBuffer, ArraySize(m_loggerFormatBuffer),
        "[%s][%-7s] %s\n", timeBuffer, GetLogSeverityName(message.GetSeverity()),
        message.GetText()) >= 0, "Failed to format epilogue");
#endif

    return m_loggerFormatBuffer;
}
