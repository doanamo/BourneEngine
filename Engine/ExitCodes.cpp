#include "Shared.hpp"
#include "ExitCodes.hpp"

const char* ExitCodeToString(ExitCodes exitCode)
{
    switch(exitCode)
    {
        case ExitCodes::Success:             return "Success";
        case ExitCodes::LoggedWarnings:      return "LoggedWarnings";
        case ExitCodes::LoggedErrors:        return "LoggedErrors";
        case ExitCodes::SetupFailed:         return "SetupFailed";
        case ExitCodes::DiscoverTestsFailed: return "DiscoverTestsFailed";
        case ExitCodes::QueryTestsFailed:    return "QueryTestsFailed";
        case ExitCodes::RunTestsFailed:      return "RunTestsFailed";
    }

    ASSERT(false, "Unknown exit code");
    return "Unknown";
}
