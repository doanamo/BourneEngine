#pragma once

enum class ExitCodes : u32
{
    Success = 0,
    LoggedWarnings,
    LoggedErrors,
    SetupFailed,
    DiscoverTestsFailed,
    QueryTestsFailed,
    RunTestsFailed,
};

const char* ExitCodeToString(ExitCodes exitCode);
