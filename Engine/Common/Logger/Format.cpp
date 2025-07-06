#include "Shared.hpp"
#include "Format.hpp"
#include "Message.hpp"
#include <ctime>

#if ENABLE_LOGGER

namespace Logger
{
    static thread_local char t_formatBuffer[Message::FormatBufferSize + 1024];

    static const char* GetSeverityName(const Severity severity)
    {
        switch(severity)
        {
            case Severity::Debug:    return "Debug";
            case Severity::Info:     return "Info";
            case Severity::Success:  return "Success";
            case Severity::Warning:  return "Warning";
            case Severity::Error:    return "Error";
            case Severity::Fatal:    return "Fatal";
        }

        ASSERT_SLOW(false, "Invalid log severity");
        return "Invalid";
    }

    static const char* ParseSourcePath(const StringView& source)
    {
        static const StringView EngineRootPath(BuildInfo::EngineRootPath);
        static const StringView EngineSourcePath(BuildInfo::EngineSourcePath);
        static const StringView ProjectSourcePath(BuildInfo::ProjectSourcePath);

        if(source.StartsWith(EngineSourcePath))
        {
            // Use an engine root path to retain the "Engine/" prefix.
            return source.GetBeginPtr() + EngineRootPath.GetLength();
        }

        if(source.StartsWith(ProjectSourcePath))
        {
            // Use a source path for foreign projects, otherwise retain the directory name.
            if(source.StartsWith(EngineRootPath))
            {
                return source.GetBeginPtr() + EngineRootPath.GetLength();
            }
            else
            {
                return source.GetBeginPtr() + ProjectSourcePath.GetLength();
            }
        }

        // This works only because we trim from front of source path string.
        return source.GetBeginPtr();
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
    if(t_writeSourceLine)
    {
        ASSERT_EVALUATE(std::snprintf(t_formatBuffer, ArraySize(t_formatBuffer),
            "[%s][%-7s] %s {%s:%u}\n", timeBuffer, GetSeverityName(message.GetSeverity()),
            message.GetText(), ParseSourcePath(message.GetSource()), message.GetLine()) >= 0,
            "Failed to format epilogue");
    }
    else
#endif
    {
        ASSERT_EVALUATE(std::snprintf(t_formatBuffer, ArraySize(t_formatBuffer),
            "[%s][%-7s] %s\n", timeBuffer, GetSeverityName(message.GetSeverity()),
            message.GetText()) >= 0, "Failed to format epilogue");
    }

    return t_formatBuffer;
}

#endif
