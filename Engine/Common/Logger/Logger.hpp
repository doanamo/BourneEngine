#pragma once

#if ENABLE_LOGGER
    #include "LoggerMessage.hpp"

    namespace Logger
    {
        void Write(const LoggerMessage& message);
        void Flush();
    };

    #if defined(CONFIG_DEBUG)
        #define LOG_MESSAGE() LoggerMessage().SetSource(__FILE__).SetLine(__LINE__)
        #define LOG_DEBUG(format, ...) Logger::Write(LOG_MESSAGE() \
            .Format(format, ## __VA_ARGS__).SetSeverity(LogSeverity::Debug))
    #else
        #define LOG_MESSAGE() LoggerMessage()
        #define LOG_DEBUG(format, ...) ((void)0)
    #endif

    #define LOG_INFO(format, ...) Logger::Write(LOG_MESSAGE() \
        .Format(format, ## __VA_ARGS__).SetSeverity(LogSeverity::Info))
    #define LOG_SUCCESS(format, ...) Logger::Write(LOG_MESSAGE() \
        .Format(format, ## __VA_ARGS__).SetSeverity(LogSeverity::Success))
    #define LOG_WARNING(format, ...) Logger::Write(LOG_MESSAGE() \
        .Format(format, ## __VA_ARGS__).SetSeverity(LogSeverity::Warning))
    #define LOG_ERROR(format, ...) Logger::Write(LOG_MESSAGE() \
        .Format(format, ## __VA_ARGS__).SetSeverity(LogSeverity::Error))
    #define LOG_FATAL(format, ...) Logger::Write(LOG_MESSAGE() \
        .Format(format, ## __VA_ARGS__).SetSeverity(LogSeverity::Fatal)); \
        DEBUG_ABORT()
    #define LOG(format, ...) LOG_INFO(format, ## __VA_ARGS__)
#else
    #define LOG_INFO(format, ...) ((void)0)
    #define LOG_SUCCESS(format, ...) ((void)0)
    #define LOG_WARNING(format, ...) ((void)0)
    #define LOG_ERROR(format, ...) ((void)0)
    // #todo: LOG_FATAL should always call abort even without logger.
    #define LOG_FATAL(format, ...) ((void)0)
    #define LOG(format, ...) ((void)0)
#endif
