#include "Shared.hpp"
#include "Memory/Stats.hpp"

static class OnStaticExit
{
public:
    ~OnStaticExit()
    {
    #if ENABLE_MEMORY_STATS
        Memory::Stats::Get().LogMemoryLeaks();
    #endif

    #if ENABLE_LOGGER
        if(Logger::GetWarningCount() > 0 || Logger::GetErrorCount() > 0)
        {
            LOG_ERROR("Exiting with %u warning(s) and %u error(s)!",
                Logger::GetWarningCount(), Logger::GetErrorCount());
        }
    #endif
    }
} g_onStaticExit;
