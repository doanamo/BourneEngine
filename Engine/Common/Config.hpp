#pragma once

namespace Common
{
    struct LoggerConfig
    {
        Logger::Severity minimumSeverity = Logger::Severity::Debug;
    };

    struct Config
    {
        LoggerConfig logger;
    };
}
