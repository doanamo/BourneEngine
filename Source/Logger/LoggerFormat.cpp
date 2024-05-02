#include "Logger/Shared.hpp"
#include "LoggerFormat.hpp"
#include <ctime>

thread_local char LoggerFormat::m_epilogueBuffer[];

#ifdef CONFIG_DEBUG
    thread_local char LoggerFormat::m_prologueBuffer[];
#endif

const char* LoggerFormat::FormatEpilogue(const LoggerMessage& message)
{
    std::time_t time = std::time(nullptr);
    std::tm* now = std::localtime(&time);

    char timeBuffer[64] = { 0 };
    ASSERT_EVALUATE(std::strftime(timeBuffer, StaticArraySize(timeBuffer),
        "%Y-%m-%d %H:%M:%S %z", now) >= 0, "Failed to format time");

    m_epilogueBuffer[0] = '\0';
    ASSERT_EVALUATE(snprintf(m_epilogueBuffer, StaticArraySize(m_epilogueBuffer),
        "[%s] ", timeBuffer) >= 0, "Failed to format epilogue");

    return m_epilogueBuffer;
}

const char* LoggerFormat::FormatPrologue(const LoggerMessage& message)
{
#ifndef CONFIG_DEBUG
    return "\n";
#else
    const char* source = message.GetSource();
    const char* sourceBegin = strstr(source, "Source\\");
    if(sourceBegin == nullptr)
    {
        sourceBegin = source;
    }

    m_prologueBuffer[0] = '\0';
    ASSERT_EVALUATE(snprintf(m_prologueBuffer, StaticArraySize(m_prologueBuffer),
        " {%s:%u}\n", sourceBegin, message.GetLine()) >= 0,
        "Failed to format prologue");
    return m_prologueBuffer;
#endif
}
