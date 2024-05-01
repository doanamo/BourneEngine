#pragma once

class LoggerMessage final
{
public:
    LoggerMessage()
    {
        m_buffer[0] = '\0';
    }

    LoggerMessage(const LoggerMessage&) = delete;
    LoggerMessage& operator=(const LoggerMessage&) = delete;

    template<typename... Arguments>
    LoggerMessage& Format(std::format_string<Arguments...> format, Arguments&&... arguments)
    {
        const u64 BufferSize = StaticArraySize(m_buffer);
        const auto result = std::format_to_n(m_buffer, BufferSize - 1,
            format, std::forward<Arguments>(arguments)...);
        *result.out = '\0';
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

private:
    static thread_local char m_buffer[1024 * 2];
    const char* m_source = nullptr;
    u32 m_line = 0;
};
