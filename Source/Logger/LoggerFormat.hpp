#pragma once

class LoggerMessage;

class LoggerFormat final
{
public:
    const char* Format(const LoggerMessage& message);

private:
    static thread_local char m_buffer[1024 * 5];
};
