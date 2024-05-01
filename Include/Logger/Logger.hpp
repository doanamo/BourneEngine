#pragma once

namespace Logger
{
    void Setup();
    void Write(const LoggerMessage& message);
};

#ifdef CONFIG_DEBUG
    #define LOG_MESSAGE() LoggerMessage().SetSource(__FILE__).SetLine(__LINE__)
#else
    #define LOG_MESSAGE() LoggerMessage()
#endif

#define LOG(format, ...) Logger::Write(LOG_MESSAGE().Format(format, ## __VA_ARGS__))
