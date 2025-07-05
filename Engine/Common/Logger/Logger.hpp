#pragma once

#if ENABLE_LOGGER

#include "Message.hpp"

namespace Logger
{
    void Write(const Message& message);
    void Flush();

    u64 GetWarningCount();
    u64 GetErrorCount();

    extern Severity g_minimumSeverity;
    extern thread_local bool t_writeSourceLine;
};

#define LOG_MINIMUM_SEVERITY_SCOPE(severity) auto UNIQUE_NAME(logMinimumSeverity) = ScopeValue(Logger::g_minimumSeverity, severity)

#if ENABLE_LOGGER_SOURCE_LINE
    #define LOG_MESSAGE() Logger::Message().SetSource(__FILE__).SetLine(__LINE__)
    #define LOG_DEBUG(format, ...) Logger::Write(LOG_MESSAGE() \
        .Format(format, ## __VA_ARGS__).SetSeverity(Logger::Severity::Debug))
    #define LOG_NO_SOURCE_LINE_SCOPE() auto UNIQUE_NAME(logNoSourceLine) = ScopeValue(Logger::t_writeSourceLine, false)
#else
    #define LOG_MESSAGE() Logger::Message()
    #define LOG_DEBUG(format, ...)
    #define LOG_NO_SOURCE_LINE_SCOPE()
#endif

#define LOG_INFO(format, ...) Logger::Write(LOG_MESSAGE() \
    .Format(format, ## __VA_ARGS__).SetSeverity(Logger::Severity::Info))
#define LOG_SUCCESS(format, ...) Logger::Write(LOG_MESSAGE() \
    .Format(format, ## __VA_ARGS__).SetSeverity(Logger::Severity::Success))
#define LOG_WARNING(format, ...) Logger::Write(LOG_MESSAGE() \
    .Format(format, ## __VA_ARGS__).SetSeverity(Logger::Severity::Warning))
#define LOG_ERROR(format, ...) Logger::Write(LOG_MESSAGE() \
    .Format(format, ## __VA_ARGS__).SetSeverity(Logger::Severity::Error))
#define LOG_FATAL(format, ...) Logger::Write(LOG_MESSAGE() \
    .Format(format, ## __VA_ARGS__).SetSeverity(Logger::Severity::Fatal)); \
    DEBUG_ABORT()
#define LOG(format, ...) LOG_INFO(format, ## __VA_ARGS__)

#else

#define LOG_MINIMUM_SEVERITY_SCOPE(severity)
#define LOG_NO_SOURCE_LINE_SCOPE()
#define LOG_DEBUG(format, ...)
#define LOG_INFO(format, ...)
#define LOG_SUCCESS(format, ...)
#define LOG_WARNING(format, ...)
#define LOG_ERROR(format, ...)
#define LOG_FATAL(format, ...) DEBUG_ABORT()
#define LOG(format, ...)

#endif
