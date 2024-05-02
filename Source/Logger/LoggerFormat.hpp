#pragma once

class LoggerMessage;

class LoggerFormat final
{
public:
    const char* FormatEpilogue(const LoggerMessage& message);
    const char* FormatPrologue(const LoggerMessage& message);

private:
    static thread_local char m_epilogueBuffer[256];

#ifdef CONFIG_DEBUG
    static thread_local char m_prologueBuffer[512];
#endif
};
