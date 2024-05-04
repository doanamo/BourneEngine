#pragma once

#include "LoggerMessage.hpp"

namespace Logger
{
    bool Setup();
    void Write(const LoggerMessage& message);
};

#ifdef CONFIG_DEBUG
    #define LOG_MESSAGE() LoggerMessage().SetSource(__FILE__).SetLine(__LINE__)
#else
    #define LOG_MESSAGE() LoggerMessage()
#endif

#ifdef CONFIG_DEBUG
    #define LOG_DEBUG(format, ...) Logger::Write(LOG_MESSAGE() \
        .Format(format, ## __VA_ARGS__).SetSeverity(LogSeverity::Debug))
#else
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
    .Format(format, ## __VA_ARGS__).SetSeverity(LogSeverity::Fatal))

#define LOG(format, ...) LOG_INFO(format, ## __VA_ARGS__)
