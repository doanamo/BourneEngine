#pragma once

enum class LogSeverity : u8
{
    Debug,
    Info,
    Success,
    Warning,
    Error,
    Fatal,
};

class LoggerMessage final
{
public:
    LoggerMessage()
    {
        m_buffer[0] = '\0';
    }

    LoggerMessage(const LoggerMessage&) = delete;
    LoggerMessage& operator=(const LoggerMessage&) = delete;

    LoggerMessage& Format(const char* format, ...)
    {
        va_list arguments;
        va_start(arguments, format);
        ASSERT_EVALUATE(vsprintf_s(m_buffer, StaticArraySize(m_buffer),
            format, arguments) >= 0, "Failed to format message");
        va_end(arguments);
        return *this;
    }
 
    LoggerMessage& SetSource(const char* source)
    {
        m_source = source;
        return *this;
    }

    LoggerMessage& SetLine(u32 line)
    {
        m_line = line;
        return *this;
    }

    LoggerMessage& SetSeverity(LogSeverity severity)
    {
        m_severity = severity;
        return *this;
    }

    const char* GetText() const
    {
        return m_buffer;
    }

    const char* GetSource() const
    {
        return m_source;
    }

    u32 GetLine() const
    {
        return m_line;
    }

    LogSeverity GetSeverity() const
    {
        return m_severity;
    }

private:
    static thread_local char m_buffer[1024 * 4];
    const char* m_source = nullptr;
    u32 m_line = 0;
    LogSeverity m_severity = LogSeverity::Info;
};
