#pragma once

#if ENABLE_LOGGER

#include "Severity.hpp"

namespace Logger
{
    class Message final
    {
    public:
        static constexpr u64 FormatBufferSize = 1024 * 4;

    private:
        static thread_local char t_buffer[FormatBufferSize];
        Severity m_severity = Severity::Info;
        const char* m_source = nullptr;
        u32 m_line = 0;

    public:
        Message();
        Message(const Message&) = delete;
        Message& operator=(const Message&) = delete;

        Message& Format(const char* format, ...);
        Message& FormatArguments(const char* format, std::va_list arguments);

        Message& SetSource(const char* source)
        {
            m_source = source;
            return *this;
        }

        Message& SetLine(const u32 line)
        {
            m_line = line;
            return *this;
        }

        Message& SetSeverity(const Severity severity)
        {
            m_severity = severity;
            return *this;
        }

        const char* GetText() const
        {
            return t_buffer;
        }

        const char* GetSource() const
        {
            return m_source;
        }

        u32 GetLine() const
        {
            return m_line;
        }

        Severity GetSeverity() const
        {
            return m_severity;
        }

        bool IsFatal() const
        {
            return m_severity == Severity::Fatal;
        }
    };
}

#endif
