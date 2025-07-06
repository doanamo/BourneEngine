#pragma once

enum class ExitCode : u32
{
    Success = 0,
    LoggedWarnings,
    LoggedErrors,
    SetupFailed,
    DiscoverTestsFailed,
    QueryTestsFailed,
    RunTestsFailed,
};

const char* ExitCodeToString(ExitCode exitCode);
