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
    static constexpr u64 FormatBufferSize = 1024 * 4;

private:
    static thread_local char m_buffer[FormatBufferSize];
    LogSeverity m_severity = LogSeverity::Info;
    const char* m_source = nullptr;
    u32 m_line = 0;

public:
    LoggerMessage();

    LoggerMessage(const LoggerMessage&) = delete;
    LoggerMessage& operator=(const LoggerMessage&) = delete;

    LoggerMessage& Format(const char* format, va_list arguments);
    LoggerMessage& Format(const char* format, ...);
 
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

    bool IsFatal() const
    {
        return m_severity == LogSeverity::Fatal;
    }
};
