#include "Shared.hpp"
#include "Format.hpp"
#include "Message.hpp"
#include <ctime>

#if ENABLE_LOGGER

namespace Logger
{
    static thread_local char t_formatBuffer[Message::FormatBufferSize + 1024];

    static const char* GetSeverityName(const Logger::Severity severity)
    {
        switch(severity)
        {
            case Logger::Severity::Debug:    return "Debug";
            case Logger::Severity::Info:     return "Info";
            case Logger::Severity::Success:  return "Success";
            case Logger::Severity::Warning:  return "Warning";
            case Logger::Severity::Error:    return "Error";
            case Logger::Severity::Fatal:    return "Fatal";
        }

        ASSERT_SLOW(false, "Invalid log severity");
        return "Invalid";
    }

    static const char* ParseSourcePath(const char* source)
    {
        // #todo: Use StringView::StartsWith() instead of strstr() across this function.
        static const bool IsEngineProject = strstr(source, BuildInfo::EngineRootPath);
        static const StringView EngineRootPath(BuildInfo::EngineRootPath);
        static const StringView ProjectSourcePath(BuildInfo::ProjectSourcePath);

        if(const char* matchBegin = strstr(source, BuildInfo::EngineSourcePath))
        {
            // Use engine root path to retain "Engine/" prefix.
            return matchBegin + EngineRootPath.GetLength();
        }

        if(const char* matchBegin = strstr(source, BuildInfo::ProjectSourcePath))
        {
            // Use source path for foreign projects, otherwise retain directory name.
            if(IsEngineProject)
            {
                return matchBegin + EngineRootPath.GetLength();
            }
            else
            {
                return matchBegin + ProjectSourcePath.GetLength();
            }
        }

        return source;
    }
}

const char* Logger::Format(const Message& message)
{
    t_formatBuffer[0] = '\0';

    const std::time_t time = std::time(nullptr);
    const std::tm* now = std::localtime(&time);

    char timeBuffer[128] = { 0 };
    ASSERT_EVALUATE(std::strftime(timeBuffer, ArraySize(timeBuffer),
        "%Y-%m-%d %H:%M:%S %z", now) > 0, "Failed to format time");

#if ENABLE_LOGGER_SOURCE_LINE
    ASSERT_EVALUATE(std::snprintf(t_formatBuffer, ArraySize(t_formatBuffer),
        "[%s][%-7s] %s {%s:%u}\n", timeBuffer, GetSeverityName(message.GetSeverity()),
        message.GetText(), ParseSourcePath(message.GetSource()), message.GetLine()) >= 0,
        "Failed to format epilogue");
#else
    ASSERT_EVALUATE(std::snprintf(t_formatBuffer, ArraySize(t_formatBuffer),
        "[%s][%-7s] %s\n", timeBuffer, GetSeverityName(message.GetSeverity()),
        message.GetText()) >= 0, "Failed to format epilogue");
#endif

    return t_formatBuffer;
}

#endif
