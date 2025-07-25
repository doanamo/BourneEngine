#include "Shared.hpp"
#include "Logger.hpp"
#include "Format.hpp"

#if ENABLE_LOGGER

Logger::Severity Logger::g_minimumSeverity = Severity::Info;
thread_local bool Logger::t_writeSourceLine = true;

std::atomic<u64> g_warningCount = 0;
std::atomic<u64> g_errorCount = 0;

void Logger::Write(const Message& message)
{
    if(message.GetSeverity() < g_minimumSeverity)
        return;

    const char* text = Format(message);

#if ENABLE_LOGGER_CONSOLE_OUTPUT
    fprintf(stdout, "%s", text);
    fflush(stdout);
#endif

    if(message.GetSeverity() == Severity::Warning)
    {
        g_warningCount.fetch_add(1, std::memory_order_relaxed);
    }
    else if(message.GetSeverity() == Severity::Error)
    {
        g_errorCount.fetch_add(1, std::memory_order_relaxed);
    }
}

void Logger::Flush()
{
#if ENABLE_LOGGER_CONSOLE_OUTPUT
    fflush(stdout);
#endif
}

u64 Logger::GetWarningCount()
{
    return g_warningCount.load();
}

u64 Logger::GetErrorCount()
{
    return g_errorCount.load();
}

#endif
