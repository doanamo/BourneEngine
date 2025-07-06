#include "Shared.hpp"
#include "ExitCode.hpp"

const char* ExitCodeToString(ExitCode exitCode)
{
    switch(exitCode)
    {
        case ExitCode::Success:             return "Success";
        case ExitCode::LoggedWarnings:      return "LoggedWarnings";
        case ExitCode::LoggedErrors:        return "LoggedErrors";
        case ExitCode::SetupFailed:         return "SetupFailed";
        case ExitCode::DiscoverTestsFailed: return "DiscoverTestsFailed";
        case ExitCode::QueryTestsFailed:    return "QueryTestsFailed";
        case ExitCode::RunTestsFailed:      return "RunTestsFailed";
    }

    ASSERT(false, "Unknown exit code");
    return "Unknown";
}
