#pragma once

#if ENABLE_LOGGER

class LoggerMessage;

namespace LoggerFormat
{
    const char* Format(const LoggerMessage& message);
};

#endif
